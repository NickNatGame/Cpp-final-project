#include "sound_processor/filters/fade_filter.h"

#include "sound_processor/common/sample_utils.h"
#include "sound_processor/filters/generators/generator_utils.h"

#include <algorithm>
#include <stdexcept>

namespace sound_processor
{

    FadeFilter::FadeFilter(Mode mode, double duration_ms) : mode_(mode), duration_ms_(duration_ms)
    {
        if (duration_ms < 0)
        {
            throw std::runtime_error("fade duration_ms must be non-negative");
        }
    }

    std::string_view FadeFilter::name() const noexcept
    {
        return mode_ == Mode::in ? "fade in" : "fade out";
    }

    void FadeFilter::apply(Waveform &waveform) const
    {
        auto &samples = waveform.samples();
        const std::size_t fade_samples = std::min(DurationMsToSampleCount(duration_ms_), samples.size());
        if (fade_samples == 0)
        {
            return;
        }

        const double denominator = fade_samples > 1 ? static_cast<double>(fade_samples - 1) : 1.0;
        for (std::size_t i = 0; i < fade_samples; ++i)
        {
            const double phase = static_cast<double>(i) / denominator;
            if (mode_ == Mode::in)
            {
                samples[i] = ClampToSample(static_cast<double>(samples[i]) * phase);
            }
            else
            {
                const std::size_t index = samples.size() - fade_samples + i;
                samples[index] = ClampToSample(static_cast<double>(samples[index]) * (1.0 - phase));
            }
        }
    }

} // namespace sound_processor
