#pragma once

#include "sound_processor/filtering/ifilter.h"

namespace sound_processor
{

/**
 * @brief Peak normalization filter.
 *
 * Scales all samples by a common factor so that
 * the largest absolute sample reaches the desired
 * peak amplitude.
 */
class NormalizeFilter final : public IFilter
{
public:
    explicit NormalizeFilter(double peak = 1.0);

    [[nodiscard]] std::string_view name() const noexcept override;
    void apply(Waveform& waveform) const override;

private:
    double peak_;
};

} // namespace sound_processor
