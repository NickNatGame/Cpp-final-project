#pragma once

#include "sound_processor/filtering/ifilter.h"

namespace sound_processor
{

/**
 * @brief Reverses waveform sample order.
 */
class ReverseFilter final : public IFilter
{
    public:
        [[nodiscard]] std::string_view name() const noexcept override;
        void apply(Waveform &waveform) const override;
};

} // namespace sound_processor
