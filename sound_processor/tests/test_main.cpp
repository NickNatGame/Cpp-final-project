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
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
    class TestFailure : public std::runtime_error
    {
    public:
        explicit TestFailure(const std::string &message) : std::runtime_error(message)
        {
        }
    };

    void Check(bool condition, const std::string &message)
    {
        if (!condition)
        {
            throw TestFailure(message);
        }
    }

    template <typename Left, typename Right>
    void CheckEqual(const Left &left, const Right &right, const std::string &message)
    {
        if (!(left == right))
        {
            throw TestFailure(message);
        }
    }

    class TestSuite
    {
    public:
        using TestFn = void (*)();

        void run(const std::string &name, TestFn test)
        {
            try
            {
                test();
                ++passed_;
                std::cout << "[PASS] " << name << '\n';
            }
            catch (const std::exception &error)
            {
                ++failed_;
                std::cerr << "[FAIL] " << name << ": " << error.what() << '\n';
            }
        }

        [[nodiscard]] int result() const noexcept
        {
            std::cout << "Passed: " << passed_ << ", failed: " << failed_ << '\n';
            return failed_ == 0 ? 0 : 1;
        }

    private:
        int passed_ = 0;
        int failed_ = 0;
    };

    void TestWaveformDuration()
    {
        sound_processor::Waveform waveform({0, 1, 2, 3}, 2);
        CheckEqual(waveform.size(), static_cast<std::size_t>(4), "Waveform size mismatch");
        Check(std::abs(waveform.durationSeconds() - 2.0) < 1e-9, "Waveform duration mismatch");
    }

    void TestArgsParser()
    {
        const char *raw_args[] = {"sound_processor", "-i", "input.wav", "-o", "output.wav", "-f", "ampl", "0.5"};
        char **argv = const_cast<char **>(raw_args);
        sound_processor::ArgsParser parser;
        const auto result = parser.parse(8, argv);
        Check(result == sound_processor::ArgsParser::Result::ok, "ArgsParser should accept valid args");
        Check(parser.args().input_file == "input.wav", "Input file not parsed");
        Check(parser.args().output_file == "output.wav", "Output file not parsed");
        CheckEqual(parser.args().filters.size(), static_cast<std::size_t>(1), "Filter count mismatch");
        CheckEqual(parser.args().filters[0].name, std::string("ampl"), "Filter name mismatch");
        CheckEqual(parser.args().filters[0].params[0], std::string("0.5"), "Filter param mismatch");
    }

    void TestAmplFilter()
    {
        sound_processor::Waveform waveform({1000, -1000, 20000});
        sound_processor::AmplFilter filter(2.0);
        filter.apply(waveform);
        CheckEqual(waveform.samples()[0], static_cast<sound_processor::Waveform::Sample>(2000), "Ampl positive sample");
        CheckEqual(waveform.samples()[1], static_cast<sound_processor::Waveform::Sample>(-2000), "Ampl negative sample");
        CheckEqual(waveform.samples()[2], static_cast<sound_processor::Waveform::Sample>(32767), "Ampl clamp sample");
    }

    void TestNormalizeFilter()
    {
        sound_processor::Waveform waveform({0, 1000, -2000});
        sound_processor::NormalizeFilter filter(0.5);
        filter.apply(waveform);
        CheckEqual(waveform.samples()[2], static_cast<sound_processor::Waveform::Sample>(-16384),
                   "Normalize target peak mismatch");
    }

    void TestPipelineFactory()
    {
        sound_processor::FilterRegistry registry;
        sound_processor::RegisterTransformFilters(registry);
        sound_processor::PipelineFactory factory;
        std::vector<sound_processor::FilterDescriptor> descriptors = {{"ampl", {"2"}}, {"reverse", {}}};
        sound_processor::Pipeline pipeline = factory.create(descriptors, registry);
        sound_processor::Waveform waveform({1, 2, 3});
        pipeline.apply(waveform);
        CheckEqual(waveform.samples()[0], static_cast<sound_processor::Waveform::Sample>(6), "Pipeline first sample");
        CheckEqual(waveform.samples()[2], static_cast<sound_processor::Waveform::Sample>(2), "Pipeline last sample");
    }

    void TestGenerator()
    {
        sound_processor::Waveform waveform;
        sound_processor::SineGeneratorFilter filter(440.0, 100.0);
        filter.apply(waveform);
        CheckEqual(waveform.size(), static_cast<std::size_t>(4410), "Generator sample count mismatch");
        Check(waveform.samples()[1] != 0, "Generator should produce non-zero samples");
    }

    void TestJsonLoader()
    {
        const std::string path = "/tmp/sound_processor_test_pipeline.json";
        std::ofstream output(path);
        output << R"({"output":"out.wav","filters":[{"name":"generator","params":["sin",440,10]},{"name":"ampl","params":[0.5]}]})";
        output.close();

        sound_processor::JsonPipelineLoader loader;
        const sound_processor::ParsedArgs args = loader.load(path);
        Check(args.output_file == "out.wav", "JSON output mismatch");
        CheckEqual(args.filters.size(), static_cast<std::size_t>(2), "JSON filter count mismatch");
        CheckEqual(args.filters[0].params[1], std::string("440"), "JSON number token mismatch");
    }

    void TestWavRoundTrip()
    {
        const std::string path = "/tmp/sound_processor_test_roundtrip.wav";
        const sound_processor::Waveform source({0, 1200, -1200, 32767, -32768});
        sound_processor::WavWriter{}.write(path, source);
        const sound_processor::Waveform restored = sound_processor::WavReader{}.read(path);
        Check(restored.samples() == source.samples(), "WAV roundtrip samples mismatch");
    }
} // namespace

int main()
{
    TestSuite suite;
    suite.run("Waveform duration", TestWaveformDuration);
    suite.run("ArgsParser", TestArgsParser);
    suite.run("AmplFilter", TestAmplFilter);
    suite.run("NormalizeFilter", TestNormalizeFilter);
    suite.run("PipelineFactory", TestPipelineFactory);
    suite.run("Generator", TestGenerator);
    suite.run("JsonLoader", TestJsonLoader);
    suite.run("WavRoundTrip", TestWavRoundTrip);
    return suite.result();
}
