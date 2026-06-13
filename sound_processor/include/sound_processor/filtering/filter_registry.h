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
    void add(std::string name, FilterProducer producer);
    [[nodiscard]] bool contains(const std::string& name) const;
    [[nodiscard]] std::unique_ptr<IFilter> create(const FilterDescriptor& descriptor) const;

private:
    std::unordered_map<std::string, FilterProducer> producers_;
};

} // namespace sound_processor
