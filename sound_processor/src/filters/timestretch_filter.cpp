#include "sound_processor/filters/timestretch_filter.h"

#include "sound_processor/common/sample_utils.h"

#include <cmath>
#include <stdexcept>
#include <vector>

namespace sound_processor
{

    TimestretchFilter::TimestretchFilter(double factor) : factor_(factor)
    {
        if (factor <= 0)
        {
            throw std::runtime_error("timestretch factor must be positive");
        }
    }

    std::string_view TimestretchFilter::name() const noexcept
    {
        return "timestretch";
    }

    void TimestretchFilter::apply(Waveform &waveform) const
    {
        const auto &source = waveform.samples();
        if (source.empty())
        {
            return;
        }

        const auto new_size = static_cast<std::size_t>(std::llround(static_cast<double>(source.size()) * factor_));
        if (new_size == 0)
        {
            waveform.clear();
            return;
        }

        std::vector<Waveform::Sample> result;
        result.reserve(new_size);
        for (std::size_t i = 0; i < new_size; ++i)
        {
            const double pos = static_cast<double>(i) / factor_;
            const auto left = static_cast<std::size_t>(std::floor(pos));
            if (left + 1 >= source.size())
            {
                result.push_back(source.back());
                continue;
            }

            const double frac = pos - static_cast<double>(left);
            const double value = static_cast<double>(source[left]) * (1.0 - frac) +
                                 static_cast<double>(source[left + 1]) * frac;
            result.push_back(ClampToSample(value));
        }

        waveform.setSamples(std::move(result));
    }

} // namespace sound_processor
