#include "sound_processor/filters/lowpass_filter.h"

#include "sound_processor/common/sample_utils.h"

#include <algorithm>
#include <stdexcept>
#include <vector>

namespace sound_processor
{

    LowpassFilter::LowpassFilter(std::size_t window_size) : window_size_(window_size)
    {
        if (window_size == 0 || window_size % 2 == 0)
        {
            throw std::runtime_error("lowpass window_size must be odd and positive");
        }
    }

    std::string_view LowpassFilter::name() const noexcept
    {
        return "lowpass";
    }

    void LowpassFilter::apply(Waveform &waveform) const
    {
        const auto &source = waveform.samples();
        if (source.empty() || window_size_ == 1)
        {
            return;
        }

        const auto radius = window_size_ / 2;
        std::vector<Waveform::Sample> result(source.size());
        for (std::size_t i = 0; i < source.size(); ++i)
        {
            long long sum = 0;
            for (std::size_t offset = 0; offset < window_size_; ++offset)
            {
                const long long raw_index = static_cast<long long>(i) + static_cast<long long>(offset) -
                                            static_cast<long long>(radius);
                const auto index = static_cast<std::size_t>(
                    std::clamp(raw_index, 0LL, static_cast<long long>(source.size() - 1)));
                sum += source[index];
            }
            result[i] = ClampToSample(static_cast<double>(sum) / static_cast<double>(window_size_));
        }

        waveform.setSamples(std::move(result));
    }

} // namespace sound_processor
