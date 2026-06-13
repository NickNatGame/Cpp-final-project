#pragma once

#include "sound_processor/waveform.h"

#include <string_view>

namespace sound_processor
{

/**
 * @brief Abstract audio processing filter.
 *
 * Base interface for all signal processing components
 * used by the application.
 */
class IFilter
{
public:
    virtual ~IFilter() = default;

    [[nodiscard]] virtual std::string_view name() const noexcept = 0;
    virtual void apply(Waveform& waveform) const = 0;
};

} // namespace sound_processor
