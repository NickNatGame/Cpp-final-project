#pragma once

#include "sound_processor/filtering/ifilter.h"

#include <cstddef>

namespace sound_processor
{

    class SilenceFilter final : public IFilter
    {
    public:
        SilenceFilter(double start_seconds, double end_seconds);

        [[nodiscard]] std::string_view name() const noexcept override;
        void apply(Waveform &waveform) const override;

    private:
        double start_seconds_;
        double end_seconds_;
    };

} // namespace sound_processor
