#include "sound_processor/filtering/filter_registry.h"

#include <stdexcept>
#include <utility>

namespace sound_processor
{

    void FilterRegistry::add(std::string name, FilterProducer producer)
    {
        if (name.empty())
        {
            throw std::invalid_argument("Filter name cannot be empty");
        }
        if (!producer)
        {
            throw std::invalid_argument("Filter producer cannot be empty");
        }

        producers_[std::move(name)] = std::move(producer);
    }

    bool FilterRegistry::contains(const std::string &name) const
    {
        return producers_.find(name) != producers_.end();
    }

    std::unique_ptr<IFilter> FilterRegistry::create(const FilterDescriptor &descriptor) const
    {
        const auto producer = producers_.find(descriptor.name);
        if (producer == producers_.end())
        {
            throw std::runtime_error("Unknown filter: " + descriptor.name);
        }

        std::unique_ptr<IFilter> filter = producer->second(descriptor);
        if (!filter)
        {
            throw std::runtime_error("Filter producer returned empty filter: " + descriptor.name);
        }
        return filter;
    }

} // namespace sound_processor
