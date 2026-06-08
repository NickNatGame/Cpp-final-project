#pragma once

#include "sound_processor/waveform.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>

namespace sound_processor
{

    template <typename Value>
    [[nodiscard]] Waveform::Sample ClampToSample(Value value)
    {
        const auto rounded = static_cast<long long>(std::llround(value));
        const auto low = static_cast<long long>(std::numeric_limits<Waveform::Sample>::min());
        const auto high = static_cast<long long>(std::numeric_limits<Waveform::Sample>::max());
        return static_cast<Waveform::Sample>(std::clamp(rounded, low, high));
    }

    [[nodiscard]] inline int SampleAbs(Waveform::Sample sample) noexcept
    {
        return sample < 0 ? -static_cast<int>(sample) : static_cast<int>(sample);
    }

} // namespace sound_processor
