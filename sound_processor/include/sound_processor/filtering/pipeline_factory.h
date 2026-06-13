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
    [[nodiscard]] Pipeline create(
        const std::vector<FilterDescriptor>& descriptors,
        const FilterRegistry& registry) const;
};

} // namespace sound_processor
