#include "sound_processor/filters/ampl_filter.h"

#include "sound_processor/common/sample_utils.h"

#include <stdexcept>

namespace sound_processor
{

    AmplFilter::AmplFilter(double factor) : factor_(factor)
    {
        if (factor < 0)
        {
            throw std::runtime_error("ampl factor must be non-negative");
        }
    }

    std::string_view AmplFilter::name() const noexcept
    {
        return "ampl";
    }

    void AmplFilter::apply(Waveform &waveform) const
    {
        for (auto &sample : waveform.samples())
        {
            sample = ClampToSample(static_cast<double>(sample) * factor_);
        }
    }

} // namespace sound_processor
