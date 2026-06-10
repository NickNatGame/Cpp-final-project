/**
 * @brief Represents a mono PCM audio waveform.
 *
 * Stores audio samples and sample rate information.
 * Provides basic operations for accessing and modifying
 * waveform data.
 */
class Waveform
{
public:
    using Sample = std::int16_t;

    /// Default sample rate (44.1 kHz).
    static constexpr std::uint32_t kDefaultSampleRate = 44100;

    /**
     * @brief Creates an empty waveform.
     */
    Waveform() = default;

    /**
     * @brief Constructs waveform from sample data.
     *
     * @param samples Audio samples.
     * @param sample_rate Sampling frequency in Hz.
     */
    explicit Waveform(std::vector<Sample> samples,
                      std::uint32_t sample_rate = kDefaultSampleRate);

    /**
     * @brief Checks whether waveform contains samples.
     *
     * @return true if empty.
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief Returns number of samples.
     */
    [[nodiscard]] std::size_t size() const noexcept;

    /**
     * @brief Returns waveform duration in seconds.
     */
    [[nodiscard]] double durationSeconds() const noexcept;

    /**
     * @brief Returns sample rate.
     */
    [[nodiscard]] std::uint32_t sampleRate() const noexcept;

    /**
     * @brief Returns read-only access to samples.
     */
    [[nodiscard]] const std::vector<Sample>& samples() const noexcept;

    /**
     * @brief Returns mutable access to samples.
     */
    [[nodiscard]] std::vector<Sample>& samples() noexcept;

    /**
     * @brief Removes all samples.
     */
    void clear() noexcept;

    /**
     * @brief Replaces waveform samples.
     *
     * @param samples New sample buffer.
     */
    void setSamples(std::vector<Sample> samples);

private:
    std::vector<Sample> samples_;
    std::uint32_t sample_rate_ = kDefaultSampleRate;
};