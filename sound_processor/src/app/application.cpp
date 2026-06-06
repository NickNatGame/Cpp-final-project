#include "sound_processor/app/application.h"

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

        const ParsedArgs &args = args_parser_.args();
        if (!args.filters.empty())
        {
            err_.error("Filters are parsed but pipeline execution is not implemented yet");
            return ResultCode::processingError;
        }

        try
        {
            return processNoFilters(args);
        }
        catch (const std::runtime_error &error)
        {
            err_.error(error.what());
            return ResultCode::ioError;
        }
    }

    void Application::printHelp() const
    {
        out_.info("Sound Processor");
        out_.info("Usage: sound_processor [-i input.wav] [-o output.wav] [-f filter [params...]]...");
        out_.info("Currently implemented: WAV read/write and command-line parsing.");
    }

    ResultCode Application::processNoFilters(const ParsedArgs &args) const
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
