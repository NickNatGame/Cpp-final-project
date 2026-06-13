#pragma once

#include "sound_processor/filtering/ifilter.h"
#include "sound_processor/waveform.h"

#include <cstddef>
#include <memory>
#include <vector>

namespace sound_processor
{

/**
 * @brief Sequence of audio filters.
 *
 * Stores filters and applies them one by one
 * to a waveform.
 */
class Pipeline
{
public:
    /** @brief Creates an empty pipeline. */
    Pipeline() = default;

    /** @brief Copying is disabled because filters are uniquely owned. */
    Pipeline(const Pipeline&) = delete;

    /** @brief Copy assignment is disabled because filters are uniquely owned. */
    Pipeline& operator=(const Pipeline&) = delete;

    /** @brief Moving transfers ownership of all filters. */
    Pipeline(Pipeline&&) noexcept = default;

    /** @brief Move assignment transfers ownership of all filters. */
    Pipeline& operator=(Pipeline&&) noexcept = default;

    /** @brief Destroys all owned filters. */
    ~Pipeline() = default;

    /**
     * @brief Adds a filter to the end of the pipeline.
     *
     * Ownership is transferred to the pipeline.
     *
     * @param filter Filter instance to store.
     * @throws std::invalid_argument if filter is null.
     */
    void add(std::unique_ptr<IFilter> filter);

    /**
     * @brief Applies all filters sequentially.
     *
     * @param waveform Waveform to process.
     */
    void apply(Waveform& waveform) const;

    /**
     * @brief Checks whether the pipeline contains no filters.
     *
     * @return true if there are no filters.
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief Returns number of filters in the pipeline.
     *
     * @return Filter count.
     */
    [[nodiscard]] std::size_t size() const noexcept;

private:
    std::vector<std::unique_ptr<IFilter>> filters_;
};

} // namespace sound_processor
