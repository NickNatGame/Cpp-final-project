#include "sound_processor/waveform.h"

#include <stdexcept>
#include <utility>

namespace sound_processor
{

    Waveform::Waveform(std::vector<Sample> samples, std::uint32_t sample_rate)
        : samples_(std::move(samples)), sample_rate_(sample_rate)
    {
        if (sample_rate_ == 0)
        {
            throw std::invalid_argument("Waveform sample rate must be positive");
        }
    }

    bool Waveform::empty() const noexcept
    {
        return samples_.empty();
    }

    std::size_t Waveform::size() const noexcept
    {
        return samples_.size();
    }

    double Waveform::durationSeconds() const noexcept
    {
        return static_cast<double>(samples_.size()) / sample_rate_;
    }

    std::uint32_t Waveform::sampleRate() const noexcept
    {
        return sample_rate_;
    }

    const std::vector<Waveform::Sample> &Waveform::samples() const noexcept
    {
        return samples_;
    }

    std::vector<Waveform::Sample> &Waveform::samples() noexcept
    {
        return samples_;
    }

    void Waveform::clear() noexcept
    {
        samples_.clear();
    }

    void Waveform::setSamples(std::vector<Sample> samples)
    {
        samples_ = std::move(samples);
    }

} // namespace sound_processor
