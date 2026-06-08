#pragma once

#include "sound_processor/waveform.h"

#include <string_view>

namespace sound_processor
{

    class IFilter
    {
    public:
        IFilter() = default;
        IFilter(const IFilter &) = default;
        IFilter &operator=(const IFilter &) = default;
        IFilter(IFilter &&) noexcept = default;
        IFilter &operator=(IFilter &&) noexcept = default;
        virtual ~IFilter() = default;

        [[nodiscard]] virtual std::string_view name() const noexcept = 0;
        virtual void apply(Waveform &waveform) const = 0;
    };

} // namespace sound_processor
