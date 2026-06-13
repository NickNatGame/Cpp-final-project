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
    enum class Mode
    {
        in,
        out
    };

    FadeFilter(Mode mode, double duration_ms);

    [[nodiscard]] std::string_view name() const noexcept override;
    void apply(Waveform& waveform) const override;

private:
    Mode mode_;
    double duration_ms_;
};

} // namespace sound_processor
