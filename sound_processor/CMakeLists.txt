add_executable(sound_processor
    src/main.cpp
)

target_include_directories(sound_processor
    PRIVATE
        include
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    target_compile_options(sound_processor
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )
endif()
