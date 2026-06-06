#include "sound_processor/app/logger.h"

#include <iostream>
#include <string_view>

namespace sound_processor
{
    namespace
    {
        std::string_view LevelName(Logger::Level level)
        {
            switch (level)
            {
            case Logger::Level::info:
                return "Info";
            case Logger::Level::warning:
                return "Warning";
            case Logger::Level::error:
                return "Error";
            }
            return "Log";
        }
    } // namespace

    Logger::Logger(std::ostream &output) : output_(output)
    {
    }

    void Logger::info(std::string_view message) const
    {
        log(Level::info, message);
    }

    void Logger::warning(std::string_view message) const
    {
        log(Level::warning, message);
    }

    void Logger::error(std::string_view message) const
    {
        log(Level::error, message);
    }

    void Logger::log(Level level, std::string_view message) const
    {
        output_ << '[' << LevelName(level) << "] " << message << '\n';
    }

} // namespace sound_processor
