#pragma once

#include "sound_processor/filtering/ifilter.h"

#include <cstddef>

namespace sound_processor
{

/**
 * @brief Simple low-pass filter based on moving average.
 */
class LowpassFilter final : public IFilter
{
public:
    /**
     * @brief Constructs a low-pass moving-average filter.
     *
     * @param window_size Averaging window size; must be positive and odd.
     * @throws std::runtime_error if window size is zero or even.
     */
    explicit LowpassFilter(std::size_t window_size);

    /**
     * @brief Returns filter name.
     *
     * @return Filter command name.
     */
    [[nodiscard]] std::string_view name() const noexcept override;

    /**
     * @brief Applies moving-average filtering.
     *
     * @param waveform Waveform to smooth.
     */
    void apply(Waveform& waveform) const override;

private:
    std::size_t window_size_;
};

} // namespace sound_processor
