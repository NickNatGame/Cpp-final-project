#pragma once

#include "sound_processor/cli/filter_descriptor.h"

#include <optional>
#include <string>
#include <vector>

namespace sound_processor
{

    struct ParsedArgs
    {
        std::optional<std::string> input_file;
        std::optional<std::string> output_file;
        std::optional<std::string> config_file;
        std::vector<FilterDescriptor> filters;
        bool help_requested = false;
    };

} // namespace sound_processor
