#pragma once

#include "sound_processor/app/logger.h"
#include "sound_processor/app/result_code.h"

#include <iosfwd>

namespace sound_processor
{

    class Application
    {
    public:
        Application(std::ostream &output, std::ostream &errors);

        void configure();
        [[nodiscard]] ResultCode start(int argc, char *argv[]);

    private:
        void printHelp() const;

        Logger out_;
        Logger err_;
        bool configured_ = false;
    };

} // namespace sound_processor
