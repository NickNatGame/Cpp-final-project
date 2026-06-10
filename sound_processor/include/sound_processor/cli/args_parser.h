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
     * @brief Parses command line arguments.
     *
     * @param argc Argument count.
     * @param argv Argument values.
     * @return Parsing status.
     */
    [[nodiscard]] Result parse(int argc, char* argv[]);

    /**
     * @brief Returns parsed arguments.
     */
    [[nodiscard]] const ParsedArgs& args() const noexcept;

    /**
     * @brief Returns error message.
     *
     * Empty if parsing succeeded.
     */
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

    /**
     * @brief Stores parsing error.
     *
     * @param message Error description.
     */
    void fail(std::string message);

private:
    ParsedArgs args_;
    std::string error_;
};