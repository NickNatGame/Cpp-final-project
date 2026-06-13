#pragma once

#include "sound_processor/filtering/ifilter.h"

namespace sound_processor
{

/**
 * @brief Fade-in and fade-out effect.
 */
class FadeFilter final : public IFilter
{
public:
    /**
     * @brief Fade operation mode.
     */
    enum class Mode
    {
        in,  ///< Fade-in effect.
        out  ///< Fade-out effect.
    };

    /**
     * @brief Constructs a fade filter.
     *
     * @param mode Fade direction.
     * @param duration_ms Fade duration in milliseconds.
     * @throws std::runtime_error if duration is negative.
     */
    FadeFilter(Mode mode, double duration_ms);

    /**
     * @brief Returns filter name.
     *
     * @return Filter command name.
     */
    [[nodiscard]] std::string_view name() const noexcept override;

    /**
     * @brief Applies fade effect to the waveform.
     *
     * @param waveform Waveform to modify.
     */
    void apply(Waveform& waveform) const override;

private:
    Mode mode_;
    double duration_ms_;
};

} // namespace sound_processor
