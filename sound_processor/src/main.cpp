#include "sound_processor/app/application.h"
#include "sound_processor/app/result_code.h"

#include <exception>
#include <iostream>

int main(int argc, char *argv[])
{
    sound_processor::Application application(std::cout, std::cerr);

    try
    {
        application.configure();
        return static_cast<int>(application.start(argc, argv));
    }
    catch (const std::exception &error)
    {
        std::cerr << "[Error] " << error.what() << '\n';
        return static_cast<int>(sound_processor::ResultCode::unhandledException);
    }
    catch (...)
    {
        std::cerr << "[Error] Unknown exception\n";
        return static_cast<int>(sound_processor::ResultCode::unknownException);
    }
}
