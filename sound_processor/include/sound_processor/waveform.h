#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace sound_processor {

class Waveform {
public:
    using Sample = std::int16_t;

    static constexpr std::uint32_t kDefaultSampleRate = 44100;

    Waveform() = default;
    explicit Waveform(std::vector<Sample> samples,
                      std::uint32_t sample_rate = kDefaultSampleRate);

    [[nodiscard]] bool empty() const noexcept;
    [[nodiscard]] std::size_t size() const noexcept;
    [[nodiscard]] double durationSeconds() const noexcept;
    [[nodiscard]] std::uint32_t sampleRate() const noexcept;

    [[nodiscard]] const std::vector<Sample>& samples() const noexcept;
    [[nodiscard]] std::vector<Sample>& samples() noexcept;

    void clear() noexcept;
    void setSamples(std::vector<Sample> samples);

private:
    std::vector<Sample> samples_;
    std::uint32_t sample_rate_ = kDefaultSampleRate;
};

}  // namespace sound_processor
