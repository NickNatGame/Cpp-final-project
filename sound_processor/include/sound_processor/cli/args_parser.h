#pragma once

#include "sound_processor/cli/parsed_args.h"

#include <optional>
#include <string>

namespace sound_processor
{

/**
 * @brief Command line argument parser.
 *
 * Parses input/output filenames, filter descriptions
 * and optional JSON configuration.
 */
class ArgsParser
{
public:
    /**
     * @brief Parsing result.
     */
    enum class Result
    {
        ok,      ///< Parsing successful.
        noArgs,  ///< No arguments provided.
        badArgs  ///< Invalid arguments.
    };

    [[nodiscard]] Result parse(int argc, char* argv[]);
    [[nodiscard]] const ParsedArgs& args() const noexcept;
    [[nodiscard]] const std::string& error() const noexcept;

private:
    static bool isKnownFlag(const std::string& token);
    static bool isFlagLike(const std::string& token);

    bool consumeFileName(
        int argc,
        char* argv[],
        int& index,
        std::optional<std::string>& target,
        const std::string& flag_name);

    bool consumeFilterDescriptor(
        int argc,
        char* argv[],
        int& index);

    bool validateConfigUsage();
    void fail(std::string message);

private:
    ParsedArgs args_;
    std::string error_;
};

} // namespace sound_processor
