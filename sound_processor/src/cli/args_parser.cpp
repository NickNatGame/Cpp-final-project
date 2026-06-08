#include "sound_processor/cli/args_parser.h"

#include <utility>

namespace sound_processor
{

    ArgsParser::Result ArgsParser::parse(int argc, char *argv[])
    {
        args_ = ParsedArgs{};
        error_.clear();

        if (argc <= 1)
        {
            return Result::noArgs;
        }

        for (int index = 1; index < argc; ++index)
        {
            const std::string token = argv[index];

            if (token == "-h" || token == "--help")
            {
                args_.help_requested = true;
            }
            else if (token == "-i")
            {
                if (!consumeFileName(argc, argv, index, args_.input_file, token))
                {
                    return Result::badArgs;
                }
            }
            else if (token == "-o")
            {
                if (!consumeFileName(argc, argv, index, args_.output_file, token))
                {
                    return Result::badArgs;
                }
            }
            else if (token == "-c" || token == "--config")
            {
                if (!consumeFileName(argc, argv, index, args_.config_file, token))
                {
                    return Result::badArgs;
                }
            }
            else if (token == "-f")
            {
                if (!consumeFilterDescriptor(argc, argv, index))
                {
                    return Result::badArgs;
                }
            }
            else
            {
                fail("Unexpected argument: " + token);
                return Result::badArgs;
            }
        }

        if (!validateConfigUsage())
        {
            return Result::badArgs;
        }

        return Result::ok;
    }

    const ParsedArgs &ArgsParser::args() const noexcept
    {
        return args_;
    }

    const std::string &ArgsParser::error() const noexcept
    {
        return error_;
    }

    bool ArgsParser::isKnownFlag(const std::string &token)
    {
        return token == "-i" || token == "-o" || token == "-f" || token == "-c" || token == "--config" ||
               token == "-h" || token == "--help";
    }

    bool ArgsParser::isFlagLike(const std::string &token)
    {
        return !token.empty() && token.front() == '-';
    }

    bool ArgsParser::consumeFileName(int argc, char *argv[], int &index, std::optional<std::string> &target,
                                     const std::string &flag_name)
    {
        if (target.has_value())
        {
            fail("Duplicate flag: " + flag_name);
            return false;
        }
        if (index + 1 >= argc || isKnownFlag(argv[index + 1]))
        {
            fail("Missing file name after " + flag_name);
            return false;
        }

        target = argv[++index];
        return true;
    }

    bool ArgsParser::consumeFilterDescriptor(int argc, char *argv[], int &index)
    {
        if (index + 1 >= argc || isKnownFlag(argv[index + 1]) || isFlagLike(argv[index + 1]))
        {
            fail("Missing filter name after -f");
            return false;
        }

        FilterDescriptor descriptor;
        descriptor.name = argv[++index];

        while (index + 1 < argc && !isKnownFlag(argv[index + 1]))
        {
            const std::string next = argv[index + 1];
            if (isFlagLike(next))
            {
                fail("Unknown flag in filter arguments: " + next);
                return false;
            }
            descriptor.params.push_back(next);
            ++index;
        }

        args_.filters.push_back(std::move(descriptor));
        return true;
    }

    bool ArgsParser::validateConfigUsage()
    {
        if (!args_.config_file.has_value())
        {
            return true;
        }
        if (args_.input_file.has_value() || args_.output_file.has_value() || !args_.filters.empty())
        {
            fail("Config mode cannot be combined with -i, -o or -f");
            return false;
        }
        return true;
    }

    void ArgsParser::fail(std::string message)
    {
        error_ = std::move(message);
    }

} // namespace sound_processor
