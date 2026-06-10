# Sound Processor

Final C++ project: a CLI WAV sound processor with a filter pipeline.

## Supported WAV format

The project intentionally supports the assignment format only:

- PCM
- mono
- 44100 Hz
- signed 16-bit samples

## Build

```bash
cmake -S . -B build
cmake --build build
```

## CLI usage

```bash
./build/sound_processor/sound_processor [-i input.wav] [-o output.wav] [-f filter [params...]]...
```

Examples:

```bash
./build/sound_processor/sound_processor -i input_example.wav -f lowpass 101 -f ampl 10 -o output_example.wav
./build/sound_processor/sound_processor -o sine.wav -f generator sin 440 2000 -f fade in 250
./build/sound_processor/sound_processor -i input_example.wav -o output_example.wav -f lowpass 5 -f normalize 0.9
```

## JSON usage

```bash
./build/sound_processor/sound_processor -c pipeline.json
```

Example JSON:

```json
{
  "output": "sine.wav",
  "filters": [
    { "name": "generator", "params": ["sin", 440, 2000] },
    { "name": "fade", "params": ["in", 250] },
    { "name": "ampl", "params": [0.8] }
  ]
}
```

## Filters

Required transform filters:

- `ampl factor`
- `normalize [peak]`
- `silence unit start end`, where `unit` is `sec` or `ms`
- `timestretch factor`
- `lowpass window_size`

Required generators:

- `generator sin frequency_hz duration_ms`
- `generator am amplitude carrier_hz modulation_hz depth duration_ms`
- `generator fm amplitude carrier_hz modulation_hz deviation_hz duration_ms`

Extra custom filters:

- `fade in duration_ms`
- `fade out duration_ms`
- `reverse`

## Architecture

The code is split by responsibility:

- `app`: application orchestration, result codes and logging
- `cli`: command-line parsing and filter descriptors
- `wav`: WAV reader/writer and waveform model
- `filtering`: `IFilter`, `Pipeline`, registry and factory
- `filters`: concrete filters and generators
- `json`: JSON pipeline loader
- `tests`: unit-style tests for core components

Filters are created through `FilterRegistry`, so adding a new filter does not require changing parser or pipeline logic.

## Tests

```bash
cmake --build build
./build/sound_processor/sound_processor_tests
cd build && ctest --output-on-failure
```
