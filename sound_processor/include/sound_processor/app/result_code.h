#pragma once

namespace sound_processor
{

    enum class ResultCode
    {
        ok = 0,
        badArguments = 1,
        ioError = 2,
        processingError = 3,
        unhandledException = 10,
        unknownException = 11
    };

} // namespace sound_processor
