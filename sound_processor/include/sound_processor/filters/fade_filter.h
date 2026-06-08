#pragma once

#include "sound_processor/filtering/ifilter.h"

#include <string>

namespace sound_processor
{

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
        void apply(Waveform &waveform) const override;

    private:
        Mode mode_;
        double duration_ms_;
    };

} // namespace sound_processor
