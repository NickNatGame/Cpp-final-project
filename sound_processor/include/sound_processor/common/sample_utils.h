#pragma once

#include "sound_processor/waveform.h"

#include <cmath>
#include <limits>

namespace sound_processor
{

/**
 * @brief Converts arbitrary numeric value into valid int16 audio sample.
 */
template <typename Value>
[[nodiscard]] Waveform::Sample ClampToSample(Value value)
{
    const auto rounded = std::llround(static_cast<long double>(value));
    const auto min_sample = static_cast<long long>(std::numeric_limits<Waveform::Sample>::min());
    const auto max_sample = static_cast<long long>(std::numeric_limits<Waveform::Sample>::max());

    if (rounded < min_sample)
    {
        return std::numeric_limits<Waveform::Sample>::min();
    }
    if (rounded > max_sample)
    {
        return std::numeric_limits<Waveform::Sample>::max();
    }
    return static_cast<Waveform::Sample>(rounded);
}

/**
 * @brief Returns absolute sample amplitude without overflowing on -32768.
 */
[[nodiscard]] inline int SampleAbs(Waveform::Sample sample) noexcept
{
    const int value = static_cast<int>(sample);
    return value < 0 ? -value : value;
}

} // namespace sound_processor
