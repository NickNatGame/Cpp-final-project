#include "sound_processor/app/application.h"

#include <ostream>
#include <stdexcept>

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
        static_cast<void>(argv);

        if (!configured_)
        {
            throw std::logic_error("Application must be configured before start");
        }

        if (argc == 1)
        {
            printHelp();
            return ResultCode::ok;
        }

        out_.info("Sound Processor application layer is ready.");
        return ResultCode::ok;
    }

    void Application::printHelp() const
    {
        out_.info("Sound Processor");
        out_.info("Usage: sound_processor [-i input.wav] [-o output.wav] [-f filter [params...]]...");
    }

} // namespace sound_processor
