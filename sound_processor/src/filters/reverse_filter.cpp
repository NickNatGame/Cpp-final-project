#include "sound_processor/filters/reverse_filter.h"

#include <algorithm>

namespace sound_processor
{

    std::string_view ReverseFilter::name() const noexcept
    {
        return "reverse";
    }

    void ReverseFilter::apply(Waveform &waveform) const
    {
        std::reverse(waveform.samples().begin(), waveform.samples().end());
    }

} // namespace sound_processor
