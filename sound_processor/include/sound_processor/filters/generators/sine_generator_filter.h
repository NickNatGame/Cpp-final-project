#pragma once

#include "sound_processor/filtering/ifilter.h"

namespace sound_processor
{

    class SineGeneratorFilter final : public IFilter
    {
    public:
        SineGeneratorFilter(double frequency_hz, double duration_ms);

        [[nodiscard]] std::string_view name() const noexcept override;
        void apply(Waveform &waveform) const override;

    private:
        double frequency_hz_;
        double duration_ms_;
    };

} // namespace sound_processor
