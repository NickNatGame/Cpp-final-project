#pragma once

#include <iosfwd>
#include <string_view>

namespace sound_processor
{

    class Logger
    {
    public:
        enum class Level
        {
            info,
            warning,
            error
        };

        explicit Logger(std::ostream &output);

        void info(std::string_view message) const;
        void warning(std::string_view message) const;
        void error(std::string_view message) const;
        void log(Level level, std::string_view message) const;

    private:
        std::ostream &output_;
    };

} // namespace sound_processor
