#pragma once

#include "sound_processor/cli/filter_descriptor.h"
#include "sound_processor/filtering/ifilter.h"

#include <functional>
#include <memory>

namespace sound_processor
{

    using FilterProducer = std::function<std::unique_ptr<IFilter>(const FilterDescriptor &)>;

} // namespace sound_processor
