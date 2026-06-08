#include "sound_processor/app/application.h"

#include "sound_processor/filtering/pipeline.h"
#include "sound_processor/waveform.h"

#include <ostream>
#include <stdexcept>
#include <string>

namespace sound_processor
{

    Application::Application(std::ostream &output, std::ostream &errors)
        : out_(output), err_(errors)
    {
    }

    void Application::configure()
    {
        configured_ = true;
    }

    ResultCode Application::start(int argc, char *argv[])
    {
        if (!configured_)
        {
            throw std::logic_error("Application must be configured before start");
        }

        const ArgsParser::Result parse_result = args_parser_.parse(argc, argv);
        if (parse_result == ArgsParser::Result::noArgs || args_parser_.args().help_requested)
        {
            printHelp();
            return ResultCode::ok;
        }
        if (parse_result == ArgsParser::Result::badArgs)
        {
            err_.error(args_parser_.error());
            printHelp();
            return ResultCode::badArguments;
        }

        try
        {
            return process(args_parser_.args());
        }
        catch (const std::runtime_error &error)
        {
            err_.error(error.what());
            return ResultCode::processingError;
        }
    }

    void Application::printHelp() const
    {
        out_.info("Sound Processor");
        out_.info("Usage: sound_processor [-i input.wav] [-o output.wav] [-f filter [params...]]...");
        out_.info("Supported filters will be registered during application configuration.");
    }

    ResultCode Application::process(const ParsedArgs &args) const
    {
        Waveform waveform;
        if (args.input_file.has_value())
        {
            out_.info("Reading input WAV: " + *args.input_file);
            waveform = wav_reader_.read(*args.input_file);
        }
        else
        {
            out_.warning("No input file specified; using an empty waveform");
        }

        Pipeline pipeline = pipeline_factory_.create(args.filters, filter_registry_);
        if (!pipeline.empty())
        {
            out_.info("Applying filters: " + std::to_string(pipeline.size()));
            pipeline.apply(waveform);
        }

        if (args.output_file.has_value())
        {
            out_.info("Writing output WAV: " + *args.output_file);
            wav_writer_.write(*args.output_file, waveform);
        }
        else
        {
            out_.warning("No output file specified; waveform is not written");
        }

        return ResultCode::ok;
    }

} // namespace sound_processor
