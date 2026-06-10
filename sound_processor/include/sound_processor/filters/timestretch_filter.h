/**
 * @brief Changes waveform duration by resampling.
 *
 * The filter uses linear interpolation between
 * neighboring samples.
 *
 * Note:
 * This implementation performs simple resampling,
 * therefore pitch changes together with duration.
 * It is not a phase-vocoder based time-stretch.
 *
 * factor > 1:
 * - waveform becomes longer
 *
 * factor < 1:
 * - waveform becomes shorter
 */
class TimestretchFilter : public IFilter
{
public:
    /**
     * @brief Constructs timestretch filter.
     *
     * @param factor Stretch factor.
     *
     * @throws std::runtime_error
     * If factor is not positive.
     */
    explicit TimestretchFilter(double factor);

    [[nodiscard]] std::string_view name() const noexcept override;

    /**
     * @brief Resamples waveform.
     *
     * @param waveform Waveform to process.
     */
    void apply(Waveform& waveform) const override;

private:
    double factor_;
};