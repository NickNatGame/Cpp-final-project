#pragma once

#include "sound_processor/cli/filter_descriptor.h"
#include "sound_processor/filtering/filter_registry.h"
#include "sound_processor/filtering/pipeline.h"

#include <vector>

namespace sound_processor
{

/**
 * @brief Builds pipelines from filter descriptors.
 */
class PipelineFactory
{
public:
    /**
     * @brief Creates a pipeline from descriptors.
     *
     * Each descriptor is converted into a concrete filter using the provided
     * registry, preserving descriptor order.
     *
     * @param descriptors Ordered filter descriptions from CLI or JSON.
     * @param registry Filter registry used to instantiate concrete filters.
     * @return Configured pipeline.
     * @throws std::runtime_error if any filter cannot be created.
     */
    [[nodiscard]] Pipeline create(
        const std::vector<FilterDescriptor>& descriptors,
        const FilterRegistry& registry) const;
};

} // namespace sound_processor
