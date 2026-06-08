#pragma once

#include "sound_processor/waveform.h"

#include <cmath>
#include <cstddef>
#include <stdexcept>

namespace sound_processor
{

    constexpr double kPi = 3.141592653589793238462643383279502884;

    [[nodiscard]] inline std::size_t DurationMsToSampleCount(double duration_ms)
    {
        if (duration_ms < 0)
        {
            throw std::runtime_error("generator duration must be non-negative");
        }
        const double seconds = duration_ms / 1000.0;
        return static_cast<std::size_t>(std::llround(seconds * Waveform::kDefaultSampleRate));
    }

} // namespace sound_processor
