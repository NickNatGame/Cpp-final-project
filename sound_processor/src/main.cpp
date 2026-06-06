#include <iostream>

namespace {

void PrintHelp() {
    std::cout
        << "Sound Processor\n"
        << "Usage:\n"
        << "  sound_processor [-i input.wav] [-o output.wav] "
        << "[-f filter [params...]]...\n";
}

}  // namespace

int main(int argc, char* argv[]) {
    static_cast<void>(argv);

    if (argc == 1) {
        PrintHelp();
        return 0;
    }

    std::cout << "Sound Processor project scaffold is ready.\n";
    return 0;
}
