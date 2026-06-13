#pragma once

#include "sound_processor/cli/filter_descriptor.h"
#include "sound_processor/filtering/filter_producer.h"
#include "sound_processor/filtering/ifilter.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace sound_processor
{

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
     * @brief Registers a filter producer under a command name.
     *
     * @param name Filter name used in CLI and JSON configuration.
     * @param producer Factory function that creates a concrete filter.
     */
    void add(std::string name, FilterProducer producer);
    /**
     * @brief Checks whether a filter name is registered.
     *
     * @param name Filter name.
     * @return true if the registry contains the filter.
     */
    [[nodiscard]] bool contains(const std::string& name) const;
    /**
     * @brief Creates a filter from a descriptor.
     *
     * @param descriptor Parsed filter name and parameters.
     * @return Owning pointer to a newly created filter.
     * @throws std::runtime_error if the filter is unknown or producer fails.
     */
    [[nodiscard]] std::unique_ptr<IFilter> create(const FilterDescriptor& descriptor) const;

private:
    std::unordered_map<std::string, FilterProducer> producers_;
};

} // namespace sound_processor
