#include "sound_processor/filtering/pipeline_factory.h"

namespace sound_processor
{

    Pipeline PipelineFactory::create(const std::vector<FilterDescriptor> &descriptors,
                                     const FilterRegistry &registry) const
    {
        Pipeline pipeline;
        for (const auto &descriptor : descriptors)
        {
            pipeline.add(registry.create(descriptor));
        }
        return pipeline;
    }

} // namespace sound_processor
