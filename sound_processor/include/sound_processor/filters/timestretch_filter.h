#pragma once

#include "sound_processor/filtering/ifilter.h"

namespace sound_processor
{

/**
 * @brief Changes waveform duration by linear resampling.
 */
class TimestretchFilter final : public IFilter
{
public:
    /**
     * @brief Constructs a timestretch filter.
     *
     * @param factor Duration multiplier.
     * @throws std::runtime_error if factor is not positive.
     */
    explicit TimestretchFilter(double factor);

    /**
     * @brief Returns filter name.
     *
     * @return Filter command name.
     */
    [[nodiscard]] std::string_view name() const noexcept override;

    /**
     * @brief Resamples waveform using linear interpolation.
     *
     * @param waveform Waveform to resample.
     */
    void apply(Waveform& waveform) const override;

private:
    double factor_;
};

} // namespace sound_processor
