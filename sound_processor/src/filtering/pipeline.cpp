#include "sound_processor/filtering/pipeline.h"

#include <stdexcept>

namespace sound_processor
{

    void Pipeline::add(std::unique_ptr<IFilter> filter)
    {
        if (!filter)
        {
            throw std::invalid_argument("Cannot add an empty filter to pipeline");
        }
        filters_.push_back(std::move(filter));
    }

    void Pipeline::apply(Waveform &waveform) const
    {
        for (const auto &filter : filters_)
        {
            filter->apply(waveform);
        }
    }

    bool Pipeline::empty() const noexcept
    {
        return filters_.empty();
    }

    std::size_t Pipeline::size() const noexcept
    {
        return filters_.size();
    }

} // namespace sound_processor
