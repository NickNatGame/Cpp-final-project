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

    /**
     * @brief Parses command-line arguments.
     *
     * @param argc Number of command-line arguments.
     * @param argv Command-line argument values.
     * @return Parsing status.
     */
    [[nodiscard]] Result parse(int argc, char* argv[]);
    /**
     * @brief Returns parsed arguments.
     *
     * @return Immutable parsed argument structure.
     */
    [[nodiscard]] const ParsedArgs& args() const noexcept;
    /**
     * @brief Returns the last parser error message.
     *
     * The string is empty when parsing succeeds.
     *
     * @return Human-readable error text.
     */
    [[nodiscard]] const std::string& error() const noexcept;

private:
    /**
     * @brief Checks whether a token is a supported top-level flag.
     *
     * @param token Command-line token.
     * @return true when token is one of the known flags.
     */
    static bool isKnownFlag(const std::string& token);
    /**
     * @brief Checks whether token syntactically looks like a flag.
     *
     * @param token Command-line token.
     * @return true when token starts with a dash.
     */
    static bool isFlagLike(const std::string& token);

    /**
     * @brief Consumes a file name after a file-related flag.
     *
     * @param argc Number of command-line arguments.
     * @param argv Command-line argument values.
     * @param index Current parser index, advanced on success.
     * @param target Destination optional file name.
     * @param flag_name Name of the flag being processed, used in errors.
     * @return true when the file name was consumed.
     */
    bool consumeFileName(
        int argc,
        char* argv[],
        int& index,
        std::optional<std::string>& target,
        const std::string& flag_name);

    /**
     * @brief Consumes a filter name and its positional parameters.
     *
     * @param argc Number of command-line arguments.
     * @param argv Command-line argument values.
     * @param index Current parser index, advanced to the next flag.
     * @return true when a descriptor was consumed.
     */
    bool consumeFilterDescriptor(
        int argc,
        char* argv[],
        int& index);

    /**
     * @brief Validates that JSON configuration mode is not mixed with CLI mode.
     *
     * @return true when the parsed mode combination is valid.
     */
    bool validateConfigUsage();
    /**
     * @brief Stores a parser failure message.
     *
     * @param message Error description.
     */
    void fail(std::string message);

private:
    ParsedArgs args_;
    std::string error_;
};

} // namespace sound_processor
