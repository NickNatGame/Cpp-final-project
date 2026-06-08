#pragma once

#include "sound_processor/filtering/ifilter.h"

namespace sound_processor
{

    class AmplFilter final : public IFilter
    {
    public:
        explicit AmplFilter(double factor);

        [[nodiscard]] std::string_view name() const noexcept override;
        void apply(Waveform &waveform) const override;

    private:
        double factor_;
    };

} // namespace sound_processor
