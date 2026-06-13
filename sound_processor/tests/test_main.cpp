#include <catch2/catch_test_macros.hpp>

#include "sound_processor/cli/args_parser.h"
#include "sound_processor/filtering/filter_registry.h"
#include "sound_processor/filtering/pipeline_factory.h"
#include "sound_processor/filtering/std_filter_producers.h"
#include "sound_processor/filters/ampl_filter.h"
#include "sound_processor/filters/generators/sine_generator_filter.h"
#include "sound_processor/filters/normalize_filter.h"
#include "sound_processor/json/json_pipeline_loader.h"
#include "sound_processor/wav_io.h"
#include "sound_processor/waveform.h"

#include <cmath>
#include <cstddef>
#include <fstream>
#include <string>
#include <vector>

TEST_CASE("Waveform calculates size and duration", "[waveform]")
{
    const sound_processor::Waveform waveform({0, 1, 2, 3}, 2);

    REQUIRE(waveform.size() == static_cast<std::size_t>(4));
    REQUIRE(std::abs(waveform.durationSeconds() - 2.0) < 1e-9);
}

TEST_CASE("ArgsParser reads files and filter descriptors", "[cli]")
{
    const char* raw_args[] = {"sound_processor", "-i", "input.wav", "-o", "output.wav", "-f", "ampl", "0.5"};
    char** argv = const_cast<char**>(raw_args);

    sound_processor::ArgsParser parser;
    const auto result = parser.parse(8, argv);

    REQUIRE(result == sound_processor::ArgsParser::Result::ok);
    REQUIRE(parser.args().input_file == "input.wav");
    REQUIRE(parser.args().output_file == "output.wav");
    REQUIRE(parser.args().filters.size() == static_cast<std::size_t>(1));
    REQUIRE(parser.args().filters[0].name == "ampl");
    REQUIRE(parser.args().filters[0].params[0] == "0.5");
}

TEST_CASE("AmplFilter scales samples and clamps overflow", "[filters]")
{
    sound_processor::Waveform waveform({1000, -1000, 20000});
    sound_processor::AmplFilter filter(2.0);

    filter.apply(waveform);

    REQUIRE(waveform.samples()[0] == static_cast<sound_processor::Waveform::Sample>(2000));
    REQUIRE(waveform.samples()[1] == static_cast<sound_processor::Waveform::Sample>(-2000));
    REQUIRE(waveform.samples()[2] == static_cast<sound_processor::Waveform::Sample>(32767));
}

TEST_CASE("NormalizeFilter reaches requested peak", "[filters]")
{
    sound_processor::Waveform waveform({0, 1000, -2000});
    sound_processor::NormalizeFilter filter(0.5);

    filter.apply(waveform);

    REQUIRE(waveform.samples()[2] == static_cast<sound_processor::Waveform::Sample>(-16384));
}

TEST_CASE("PipelineFactory creates and applies a filter chain", "[pipeline]")
{
    sound_processor::FilterRegistry registry;
    sound_processor::RegisterTransformFilters(registry);
    const sound_processor::PipelineFactory factory;
    const std::vector<sound_processor::FilterDescriptor> descriptors = {{"ampl", {"2"}}, {"reverse", {}}};

    sound_processor::Pipeline pipeline = factory.create(descriptors, registry);
    sound_processor::Waveform waveform({1, 2, 3});
    pipeline.apply(waveform);

    REQUIRE(waveform.samples()[0] == static_cast<sound_processor::Waveform::Sample>(6));
    REQUIRE(waveform.samples()[2] == static_cast<sound_processor::Waveform::Sample>(2));
}

TEST_CASE("Sine generator creates samples from an empty waveform", "[generators]")
{
    sound_processor::Waveform waveform;
    sound_processor::SineGeneratorFilter filter(440.0, 100.0);

    filter.apply(waveform);

    REQUIRE(waveform.size() == static_cast<std::size_t>(4410));
    REQUIRE(waveform.samples()[1] != 0);
}

TEST_CASE("JsonPipelineLoader reads output and filter params", "[json]")
{
    const std::string path = "/tmp/sound_processor_test_pipeline.json";
    std::ofstream output(path);
    output << R"({"output":"out.wav","filters":[{"name":"generator","params":["sin",440,10]},{"name":"ampl","params":[0.5]}]})";
    output.close();

    const sound_processor::JsonPipelineLoader loader;
    const sound_processor::ParsedArgs args = loader.load(path);

    REQUIRE(args.output_file == "out.wav");
    REQUIRE(args.filters.size() == static_cast<std::size_t>(2));
    REQUIRE(args.filters[0].params[1] == "440");
}

TEST_CASE("WAV writer and reader preserve samples", "[wav]")
{
    const std::string path = "/tmp/sound_processor_test_roundtrip.wav";
    const sound_processor::Waveform source({0, 1200, -1200, 32767, -32768});

    sound_processor::WavWriter{}.write(path, source);
    const sound_processor::Waveform restored = sound_processor::WavReader{}.read(path);

    REQUIRE(restored.samples() == source.samples());
}
