#pragma once

#include "sound_processor/waveform.h"

#include <string>

namespace sound_processor {

class WavReader {
public:
    [[nodiscard]] Waveform read(const std::string& file_name) const;
};

class WavWriter {
public:
    void write(const std::string& file_name, const Waveform& waveform) const;
};

}  // namespace sound_processor
