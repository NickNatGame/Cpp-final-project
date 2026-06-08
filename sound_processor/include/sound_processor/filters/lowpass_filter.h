#pragma once

#include "sound_processor/filtering/ifilter.h"

#include <cstddef>

namespace sound_processor
{

    class LowpassFilter final : public IFilter
    {
    public:
        explicit LowpassFilter(std::size_t window_size);

        [[nodiscard]] std::string_view name() const noexcept override;
        void apply(Waveform &waveform) const override;

    private:
        std::size_t window_size_;
    };

} // namespace sound_processor
