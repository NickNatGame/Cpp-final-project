#pragma once

#include "sound_processor/cli/parsed_args.h"

#include <string>

namespace sound_processor
{

    class JsonPipelineLoader
    {
    public:
        [[nodiscard]] ParsedArgs load(const std::string &file_name) const;
    };

} // namespace sound_processor
