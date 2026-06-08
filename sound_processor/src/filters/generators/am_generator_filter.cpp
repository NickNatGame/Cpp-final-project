#include "sound_processor/filters/generators/am_generator_filter.h"

#include "sound_processor/common/sample_utils.h"
#include "sound_processor/filters/generators/generator_utils.h"

#include <cmath>
#include <stdexcept>
#include <vector>

namespace sound_processor
{

    AmGeneratorFilter::AmGeneratorFilter(double amplitude, double carrier_hz, double modulation_hz, double depth,
                                         double duration_ms)
        : amplitude_(amplitude), carrier_hz_(carrier_hz), modulation_hz_(modulation_hz), depth_(depth),
          duration_ms_(duration_ms)
    {
        if (amplitude < 0 || amplitude > 1)
        {
            throw std::runtime_error("generator am amplitude must be in range [0, 1]");
        }
        if (carrier_hz < 0 || modulation_hz < 0)
        {
            throw std::runtime_error("generator am frequencies must be non-negative");
        }
        if (depth < 0 || depth > 1)
        {
            throw std::runtime_error("generator am depth must be in range [0, 1]");
        }
        if (duration_ms < 0)
        {
            throw std::runtime_error("generator am duration_ms must be non-negative");
        }
    }

    std::string_view AmGeneratorFilter::name() const noexcept
    {
        return "generator am";
    }

    void AmGeneratorFilter::apply(Waveform &waveform) const
    {
        const std::size_t sample_count = DurationMsToSampleCount(duration_ms_);
        std::vector<Waveform::Sample> samples;
        samples.reserve(sample_count);

        for (std::size_t i = 0; i < sample_count; ++i)
        {
            const double t = static_cast<double>(i) / Waveform::kDefaultSampleRate;
            const double envelope = 1.0 + depth_ * std::sin(2.0 * kPi * modulation_hz_ * t);
            const double carrier = std::sin(2.0 * kPi * carrier_hz_ * t);
            samples.push_back(ClampToSample(amplitude_ * 32767.0 * envelope * carrier));
        }

        waveform = Waveform(std::move(samples));
    }

} // namespace sound_processor
