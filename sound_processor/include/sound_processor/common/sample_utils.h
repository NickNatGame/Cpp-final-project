/**
 * @brief Converts arbitrary numeric value
 * into valid audio sample.
 *
 * Value is rounded and clamped to the
 * int16_t sample range.
 *
 * @tparam Value Numeric type.
 * @param value Input value.
 * @return Clamped sample.
 */
template <typename Value>
[[nodiscard]] Waveform::Sample ClampToSample(Value value);

/**
 * @brief Returns absolute sample amplitude.
 *
 * @param sample Audio sample.
 * @return Absolute value.
 */
[[nodiscard]] inline int SampleAbs(Waveform::Sample sample) noexcept;