#pragma once

#include "sound_processor/app/logger.h"
#include "sound_processor/app/result_code.h"
#include "sound_processor/cli/args_parser.h"
#include "sound_processor/cli/parsed_args.h"
#include "sound_processor/wav_io.h"

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
        [[nodiscard]] ResultCode processNoFilters(const ParsedArgs &args) const;

        Logger out_;
        Logger err_;
        ArgsParser args_parser_;
        WavReader wav_reader_;
        WavWriter wav_writer_;
        bool configured_ = false;
    };

} // namespace sound_processor
