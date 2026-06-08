#include "sound_processor/filters/normalize_filter.h"

#include "sound_processor/common/sample_utils.h"

#include <algorithm>
#include <limits>
#include <stdexcept>

namespace sound_processor
{

    NormalizeFilter::NormalizeFilter(double peak) : peak_(peak)
    {
        if (peak < 0 || peak > 1)
        {
            throw std::runtime_error("normalize peak must be in range [0, 1]");
        }
    }

    std::string_view NormalizeFilter::name() const noexcept
    {
        return "normalize";
    }

    void NormalizeFilter::apply(Waveform &waveform) const
    {
        int current_peak = 0;
        for (const auto sample : waveform.samples())
        {
            current_peak = std::max(current_peak, SampleAbs(sample));
        }

        if (current_peak == 0)
        {
            return;
        }

        const double scale = peak_ * std::numeric_limits<Waveform::Sample>::max() / current_peak;
        for (auto &sample : waveform.samples())
        {
            sample = ClampToSample(static_cast<double>(sample) * scale);
        }
    }

} // namespace sound_processor
