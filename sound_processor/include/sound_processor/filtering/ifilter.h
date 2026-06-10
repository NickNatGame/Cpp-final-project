/**
 * @brief Abstract audio processing filter.
 *
 * Base interface for all signal processing components
 * used by the application.
 *
 * Every filter receives a waveform and may modify it
 * in-place. Examples include amplification, normalization,
 * low-pass filtering, silence insertion, signal generation
 * and time stretching.
 *
 * Implementations are expected to provide strong exception
 * safety guarantees. If an exception is thrown during
 * processing, the waveform should remain valid.
 */
class IFilter
{
public:
    virtual ~IFilter() = default;

    /**
     * @brief Returns human-readable filter name.
     *
     * Used for logging and diagnostics.
     *
     * @return Filter name.
     */
    [[nodiscard]] virtual std::string_view name() const noexcept = 0;

    /**
     * @brief Applies filter to waveform.
     *
     * Implementations may modify sample values,
     * waveform length or completely replace
     * waveform contents.
     *
     * @param waveform Waveform to process.
     */
    virtual void apply(Waveform& waveform) const = 0;
};