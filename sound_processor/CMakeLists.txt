set(SOUND_PROCESSOR_CORE_SOURCES
    src/app/application.cpp
    src/app/logger.cpp
    src/cli/args_parser.cpp
    src/filtering/filter_registry.cpp
    src/filtering/pipeline.cpp
    src/filtering/pipeline_factory.cpp
    src/filtering/std_filter_producers.cpp
    src/json/json_pipeline_loader.cpp
    src/filters/ampl_filter.cpp
    src/filters/generators/am_generator_filter.cpp
    src/filters/generators/fm_generator_filter.cpp
    src/filters/generators/sine_generator_filter.cpp
    src/filters/lowpass_filter.cpp
    src/filters/normalize_filter.cpp
    src/filters/reverse_filter.cpp
    src/filters/silence_filter.cpp
    src/filters/timestretch_filter.cpp
    src/filters/fade_filter.cpp
    src/waveform.cpp
    src/wav_io.cpp
)

add_library(sound_processor_core STATIC
    ${SOUND_PROCESSOR_CORE_SOURCES}
)

target_include_directories(sound_processor_core
    PUBLIC
        include
)

add_executable(sound_processor
    src/main.cpp
)

target_link_libraries(sound_processor
    PRIVATE
        sound_processor_core
)

add_executable(sound_processor_tests
    tests/test_main.cpp
)

target_link_libraries(sound_processor_tests
    PRIVATE
        sound_processor_core
)

add_test(NAME sound_processor_tests COMMAND sound_processor_tests)

foreach(target sound_processor_core sound_processor sound_processor_tests)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
        target_compile_options(${target}
            PRIVATE
                -Wall
                -Wextra
                -Wpedantic
        )
    endif()
endforeach()
