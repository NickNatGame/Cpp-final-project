/**
 * @brief Fade-in and fade-out effect.
 *
 * Gradually changes signal amplitude over
 * a specified time interval.
 *
 * Fade-in:
 * @code
 * 0% -> 100%
 * @endcode
 *
 * Fade-out:
 * @code
 * 100% -> 0%
 * @endcode
 */
class FadeFilter : public IFilter
{
public:
    /**
     * @brief Fade operation mode.
     */
    enum class Mode
    {
        in,   ///< Fade-in effect.
        out   ///< Fade-out effect.
    };

    /**
     * @brief Constructs fade filter.
     *
     * @param mode Fade mode.
     * @param duration_ms Fade duration.
     *
     * @throws std::runtime_error
     * If duration is negative.
     */
    FadeFilter(Mode mode, double duration_ms);

    [[nodiscard]] std::string_view name() const noexcept override;

    /**
     * @brief Applies fade effect.
     *
     * @param waveform Waveform to process.
     */
    void apply(Waveform& waveform) const override;

private:
    Mode mode_;
    double duration_ms_;
};