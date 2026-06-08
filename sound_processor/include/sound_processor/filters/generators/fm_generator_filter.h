#pragma once

#include "sound_processor/filtering/ifilter.h"

namespace sound_processor
{

    class FmGeneratorFilter final : public IFilter
    {
    public:
        FmGeneratorFilter(double amplitude, double carrier_hz, double modulation_hz, double deviation_hz,
                          double duration_ms);

        [[nodiscard]] std::string_view name() const noexcept override;
        void apply(Waveform &waveform) const override;

    private:
        double amplitude_;
        double carrier_hz_;
        double modulation_hz_;
        double deviation_hz_;
        double duration_ms_;
    };

} // namespace sound_processor
