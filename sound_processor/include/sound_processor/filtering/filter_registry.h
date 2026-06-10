/**
 * @brief Registry of available filters.
 *
 * Maps filter names to factory functions that
 * create filter instances.
 */
class FilterRegistry
{
public:
    /**
     * @brief Registers a filter factory.
     *
     * @param name Filter name.
     * @param producer Factory function.
     */
    void add(std::string name, FilterProducer producer);

    /**
     * @brief Checks whether filter exists.
     *
     * @param name Filter name.
     * @return true if registered.
     */
    [[nodiscard]] bool contains(const std::string& name) const;

    /**
     * @brief Creates filter instance.
     *
     * @param descriptor Filter description.
     * @return Newly created filter.
     *
     * @throws std::runtime_error if filter is unknown.
     */
    [[nodiscard]] std::unique_ptr<IFilter>
    create(const FilterDescriptor& descriptor) const;

private:
    std::unordered_map<std::string, FilterProducer> producers_;
};