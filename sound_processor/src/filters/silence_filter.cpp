#include "sound_processor/filters/silence_filter.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>

namespace sound_processor
{

    SilenceFilter::SilenceFilter(double start_seconds, double end_seconds)
        : start_seconds_(start_seconds), end_seconds_(end_seconds)
    {
        if (start_seconds < 0)
        {
            throw std::runtime_error("silence start must be non-negative");
        }
        if (end_seconds < start_seconds)
        {
            throw std::runtime_error("silence end must be greater than or equal to start");
        }
    }

    std::string_view SilenceFilter::name() const noexcept
    {
        return "silence";
    }

    void SilenceFilter::apply(Waveform &waveform) const
    {
        const auto sample_rate = static_cast<double>(waveform.sampleRate());
        const auto start_index = static_cast<std::size_t>(std::llround(start_seconds_ * sample_rate));
        const auto end_index = static_cast<std::size_t>(std::llround(end_seconds_ * sample_rate));
        const auto silence_size = end_index - start_index + 1;
        const auto insert_pos = std::min(start_index, waveform.size());

        auto samples = waveform.samples();
        samples.insert(samples.begin() + static_cast<std::ptrdiff_t>(insert_pos), silence_size, 0);
        waveform.setSamples(std::move(samples));
    }

} // namespace sound_processor
