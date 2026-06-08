#pragma once

#include "sound_processor/filtering/ifilter.h"

#include <cstddef>
#include <memory>
#include <vector>

namespace sound_processor
{

    class Pipeline
    {
    public:
        Pipeline() = default;
        Pipeline(const Pipeline &) = delete;
        Pipeline &operator=(const Pipeline &) = delete;
        Pipeline(Pipeline &&) noexcept = default;
        Pipeline &operator=(Pipeline &&) noexcept = default;
        ~Pipeline() = default;

        void add(std::unique_ptr<IFilter> filter);
        void apply(Waveform &waveform) const;

        [[nodiscard]] bool empty() const noexcept;
        [[nodiscard]] std::size_t size() const noexcept;

    private:
        std::vector<std::unique_ptr<IFilter>> filters_;
    };

} // namespace sound_processor
