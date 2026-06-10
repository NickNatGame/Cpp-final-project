/**
 * @brief Builds pipelines from filter descriptors.
 */
class PipelineFactory
{
public:
    /**
     * @brief Creates pipeline from descriptors.
     *
     * Each descriptor is converted into a filter
     * using the provided registry.
     *
     * @param descriptors Filter descriptions.
     * @param registry Filter registry.
     * @return Configured pipeline.
     */
    [[nodiscard]] Pipeline create(
        const std::vector<FilterDescriptor>& descriptors,
        const FilterRegistry& registry) const;
};