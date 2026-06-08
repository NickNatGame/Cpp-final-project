#pragma once

#include "sound_processor/filtering/filter_producer.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace sound_processor
{

    class FilterRegistry
    {
    public:
        void add(std::string name, FilterProducer producer);
        [[nodiscard]] bool contains(const std::string &name) const;
        [[nodiscard]] std::unique_ptr<IFilter> create(const FilterDescriptor &descriptor) const;

    private:
        std::unordered_map<std::string, FilterProducer> producers_;
    };

} // namespace sound_processor
