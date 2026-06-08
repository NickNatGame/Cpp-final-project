#pragma once

#include "sound_processor/filtering/ifilter.h"

namespace sound_processor
{

    class AmGeneratorFilter final : public IFilter
    {
    public:
        AmGeneratorFilter(double amplitude, double carrier_hz, double modulation_hz, double depth,
                          double duration_ms);

        [[nodiscard]] std::string_view name() const noexcept override;
        void apply(Waveform &waveform) const override;

    private:
        double amplitude_;
        double carrier_hz_;
        double modulation_hz_;
        double depth_;
        double duration_ms_;
    };

} // namespace sound_processor
