/**
 * @brief Sequence of audio filters.
 *
 * Stores filters and applies them one by one
 * to a waveform.
 */
class Pipeline
{
public:
    Pipeline() = default;
    Pipeline(const Pipeline&) = delete;
    Pipeline& operator=(const Pipeline&) = delete;
    Pipeline(Pipeline&&) noexcept = default;
    Pipeline& operator=(Pipeline&&) noexcept = default;
    ~Pipeline() = default;

    /**
     * @brief Adds filter to pipeline.
     *
     * Ownership is transferred to the pipeline.
     *
     * @param filter Filter instance.
     */
    void add(std::unique_ptr<IFilter> filter);

    /**
     * @brief Applies all filters sequentially.
     *
     * @param waveform Waveform to process.
     */
    void apply(Waveform& waveform) const;

    /**
     * @brief Checks whether pipeline contains filters.
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief Returns filter count.
     */
    [[nodiscard]] std::size_t size() const noexcept;

private:
    std::vector<std::unique_ptr<IFilter>> filters_;
};