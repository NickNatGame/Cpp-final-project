#include "sound_processor/filters/generators/sine_generator_filter.h"

#include "sound_processor/common/sample_utils.h"
#include "sound_processor/filters/generators/generator_utils.h"

#include <cmath>
#include <stdexcept>
#include <vector>

namespace sound_processor
{

    SineGeneratorFilter::SineGeneratorFilter(double frequency_hz, double duration_ms)
        : frequency_hz_(frequency_hz), duration_ms_(duration_ms)
    {
        if (frequency_hz < 0)
        {
            throw std::runtime_error("generator sin frequency_hz must be non-negative");
        }
        if (duration_ms < 0)
        {
            throw std::runtime_error("generator sin duration_ms must be non-negative");
        }
    }

    std::string_view SineGeneratorFilter::name() const noexcept
    {
        return "generator sin";
    }

    void SineGeneratorFilter::apply(Waveform &waveform) const
    {
        const std::size_t sample_count = DurationMsToSampleCount(duration_ms_);
        std::vector<Waveform::Sample> samples;
        samples.reserve(sample_count);

        for (std::size_t i = 0; i < sample_count; ++i)
        {
            const double t = static_cast<double>(i) / Waveform::kDefaultSampleRate;
            const double value = 32767.0 * std::sin(2.0 * kPi * frequency_hz_ * t);
            samples.push_back(ClampToSample(value));
        }

        waveform = Waveform(std::move(samples));
    }

} // namespace sound_processor
