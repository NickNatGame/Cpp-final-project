/**
 * @brief Simple low-pass filter based on moving average.
 *
 * Replaces each sample with the arithmetic mean
 * of neighboring samples inside a sliding window.
 *
 * This suppresses high-frequency components and
 * smooths rapid changes in the waveform.
 *
 * The implementation uses edge replication near
 * waveform boundaries.
 *
 * Example:
 *
 * Original:
 * @code
 * 0 0 100 0 0
 * @endcode
 *
 * Filtered:
 * @code
 * 0 33 33 33 0
 * @endcode
 */
class LowpassFilter : public IFilter
{
public:
    /**
     * @brief Constructs low-pass filter.
     *
     * @param window_size Averaging window size.
     *
     * Must be odd and positive.
     *
     * @throws std::runtime_error
     * If window size is zero or even.
     */
    explicit LowpassFilter(std::size_t window_size);

    [[nodiscard]] std::string_view name() const noexcept override;

    /**
     * @brief Applies moving-average filtering.
     *
     * @param waveform Waveform to filter.
     */
    void apply(Waveform& waveform) const override;

private:
    std::size_t window_size_;
};