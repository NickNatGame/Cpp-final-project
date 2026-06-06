#pragma once

#include <string>
#include <vector>

namespace sound_processor
{

    struct FilterDescriptor
    {
        std::string name;
        std::vector<std::string> params;
    };

} // namespace sound_processor
