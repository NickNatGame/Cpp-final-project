/**
 * @brief Peak normalization filter.
 *
 * Scales all samples by a common factor so that
 * the largest absolute sample reaches the desired
 * peak amplitude.
 *
 * Signal shape is preserved because every sample
 * is multiplied by the same coefficient.
 *
 * This filter changes volume but does not alter
 * frequency content.
 */
class NormalizeFilter : public IFilter
{
public:
    /**
     * @brief Constructs normalization filter.
     *
     * @param peak Desired peak level.
     *
     * Valid range:
     * @code
     * [0.0, 1.0]
     * @endcode
     *
     * 1.0 corresponds to the maximum int16_t value.
     *
     * @throws std::runtime_error
     * If peak is outside allowed range.
     */
    explicit NormalizeFilter(double peak = 1.0);

    [[nodiscard]] std::string_view name() const noexcept override;

    /**
     * @brief Normalizes waveform amplitude.
     *
     * If waveform contains only silence,
     * no action is performed.
     *
     * @param waveform Waveform to normalize.
     */
    void apply(Waveform& waveform) const override;

private:
    double peak_;
};