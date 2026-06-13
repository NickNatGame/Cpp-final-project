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
    /**
     * @brief Destroys a filter through the base interface.
     */
    virtual ~IFilter() = default;

    /**
     * @brief Returns human-readable filter name.
     *
     * Used for diagnostics, logging and debugging filter pipelines.
     *
     * @return Filter name.
     */
    [[nodiscard]] virtual std::string_view name() const noexcept = 0;
    /**
     * @brief Applies the filter to a waveform.
     *
     * @param waveform Waveform to process.
     */
    virtual void apply(Waveform& waveform) const = 0;
};

} // namespace sound_processor
