#pragma once

#include "sound_processor/filtering/ifilter.h"

namespace sound_processor
{

/**
 * @brief Multiplies waveform samples by a constant factor.
 *
 * The result is rounded and clamped to the valid int16_t sample range.
 */
class AmplFilter final : public IFilter
{
public:
    /**
     * @brief Creates an amplification filter.
     *
     * @param factor Multiplication coefficient.
     * @throws std::runtime_error if factor is negative.
     */
    explicit AmplFilter(double factor);

    /**
     * @brief Returns filter name.
     *
     * @return Filter command name.
     */
    [[nodiscard]] std::string_view name() const noexcept override;

    /**
     * @brief Applies amplification to every sample.
     *
     * @param waveform Waveform to modify.
     */
    void apply(Waveform& waveform) const override;

private:
    double factor_;
};

} // namespace sound_processor
