#include "sound_processor/filters/generators/fm_generator_filter.h"

#include "sound_processor/common/sample_utils.h"
#include "sound_processor/filters/generators/generator_utils.h"

#include <cmath>
#include <stdexcept>
#include <vector>

namespace sound_processor
{

    FmGeneratorFilter::FmGeneratorFilter(double amplitude, double carrier_hz, double modulation_hz,
                                         double deviation_hz, double duration_ms)
        : amplitude_(amplitude), carrier_hz_(carrier_hz), modulation_hz_(modulation_hz),
          deviation_hz_(deviation_hz), duration_ms_(duration_ms)
    {
        if (amplitude < 0 || amplitude > 1)
        {
            throw std::runtime_error("generator fm amplitude must be in range [0, 1]");
        }
        if (carrier_hz < 0)
        {
            throw std::runtime_error("generator fm carrier_hz must be non-negative");
        }
        if (modulation_hz <= 0)
        {
            throw std::runtime_error("generator fm modulation_hz must be positive");
        }
        if (deviation_hz < 0)
        {
            throw std::runtime_error("generator fm deviation_hz must be non-negative");
        }
        if (duration_ms < 0)
        {
            throw std::runtime_error("generator fm duration_ms must be non-negative");
        }
    }

    std::string_view FmGeneratorFilter::name() const noexcept
    {
        return "generator fm";
    }

    void FmGeneratorFilter::apply(Waveform &waveform) const
    {
        const std::size_t sample_count = DurationMsToSampleCount(duration_ms_);
        std::vector<Waveform::Sample> samples;
        samples.reserve(sample_count);

        for (std::size_t i = 0; i < sample_count; ++i)
        {
            const double t = static_cast<double>(i) / Waveform::kDefaultSampleRate;
            const double phase = 2.0 * kPi * carrier_hz_ * t +
                                 (deviation_hz_ / modulation_hz_) * std::sin(2.0 * kPi * modulation_hz_ * t);
            samples.push_back(ClampToSample(amplitude_ * 32767.0 * std::sin(phase)));
        }

        waveform = Waveform(std::move(samples));
    }

} // namespace sound_processor
