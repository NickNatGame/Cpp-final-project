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
    /**
     * @brief Constructs normalization filter.
     *
     * @param peak Target peak in range [0.0, 1.0].
     * @throws std::runtime_error if peak is outside the allowed range.
     */
    explicit NormalizeFilter(double peak = 1.0);

    /**
     * @brief Returns filter name.
     *
     * @return Filter command name.
     */
    [[nodiscard]] std::string_view name() const noexcept override;

    /**
     * @brief Normalizes waveform amplitude.
     *
     * If the waveform is empty or contains only silence, no action is performed.
     *
     * @param waveform Waveform to normalize.
     */
    void apply(Waveform& waveform) const override;

private:
    double peak_;
};

} // namespace sound_processor
