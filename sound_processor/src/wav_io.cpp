#include "sound_processor/wav_io.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace sound_processor
{
    namespace
    {

        constexpr std::uint16_t kPcmFormat = 1;
        constexpr std::uint16_t kMonoChannels = 1;
        constexpr std::uint16_t kBitsPerSample = 16;
        constexpr std::uint16_t kBlockAlign = kMonoChannels * kBitsPerSample / 8;
        constexpr std::uint32_t kBytesPerSecond = Waveform::kDefaultSampleRate * kBlockAlign;

        struct FormatChunk
        {
            std::uint16_t audio_format = 0;
            std::uint16_t channels = 0;
            std::uint32_t sample_rate = 0;
            std::uint32_t byte_rate = 0;
            std::uint16_t block_align = 0;
            std::uint16_t bits_per_sample = 0;
        };

        void ReadExact(std::istream &input, char *data, std::streamsize size,
                       std::string_view field_name)
        {
            input.read(data, size);
            if (input.gcount() != size)
            {
                throw std::runtime_error("Unexpected end of WAV file while reading " +
                                         std::string(field_name));
            }
        }

        bool TryReadTag(std::istream &input, std::array<char, 4> &tag)
        {
            input.read(tag.data(), static_cast<std::streamsize>(tag.size()));
            if (input.gcount() == 0 && input.eof())
            {
                return false;
            }
            if (input.gcount() != static_cast<std::streamsize>(tag.size()))
            {
                throw std::runtime_error("Unexpected end of WAV file while reading chunk tag");
            }
            return true;
        }

        std::array<char, 4> ReadTag(std::istream &input, std::string_view field_name)
        {
            std::array<char, 4> tag{};
            ReadExact(input, tag.data(), static_cast<std::streamsize>(tag.size()), field_name);
            return tag;
        }

        bool TagEquals(const std::array<char, 4> &tag, std::string_view expected)
        {
            return expected.size() == tag.size() &&
                   std::equal(tag.begin(), tag.end(), expected.begin());
        }

        std::uint16_t ReadU16(std::istream &input, std::string_view field_name)
        {
            std::array<unsigned char, 2> bytes{};
            ReadExact(input, reinterpret_cast<char *>(bytes.data()),
                      static_cast<std::streamsize>(bytes.size()), field_name);
            return static_cast<std::uint16_t>(bytes[0]) |
                   static_cast<std::uint16_t>(bytes[1] << 8);
        }

        std::uint32_t ReadU32(std::istream &input, std::string_view field_name)
        {
            std::array<unsigned char, 4> bytes{};
            ReadExact(input, reinterpret_cast<char *>(bytes.data()),
                      static_cast<std::streamsize>(bytes.size()), field_name);
            return static_cast<std::uint32_t>(bytes[0]) |
                   (static_cast<std::uint32_t>(bytes[1]) << 8) |
                   (static_cast<std::uint32_t>(bytes[2]) << 16) |
                   (static_cast<std::uint32_t>(bytes[3]) << 24);
        }

        Waveform::Sample ReadI16(std::istream &input, std::string_view field_name)
        {
            return static_cast<Waveform::Sample>(ReadU16(input, field_name));
        }

        void SkipBytes(std::istream &input, std::uint32_t byte_count)
        {
            input.seekg(static_cast<std::streamoff>(byte_count), std::ios::cur);
            if (!input)
            {
                throw std::runtime_error("Unexpected end of WAV file while skipping chunk data");
            }
        }

        FormatChunk ReadFormatChunk(std::istream &input, std::uint32_t chunk_size)
        {
            if (chunk_size < 16)
            {
                throw std::runtime_error("Invalid WAV format chunk size");
            }

            FormatChunk format{};
            format.audio_format = ReadU16(input, "audio format");
            format.channels = ReadU16(input, "channel count");
            format.sample_rate = ReadU32(input, "sample rate");
            format.byte_rate = ReadU32(input, "byte rate");
            format.block_align = ReadU16(input, "block align");
            format.bits_per_sample = ReadU16(input, "bits per sample");

            if (chunk_size > 16)
            {
                SkipBytes(input, chunk_size - 16);
            }
            return format;
        }

        void ValidateFormat(const FormatChunk &format)
        {
            if (format.audio_format != kPcmFormat || format.channels != kMonoChannels ||
                format.sample_rate != Waveform::kDefaultSampleRate ||
                format.byte_rate != kBytesPerSecond || format.block_align != kBlockAlign ||
                format.bits_per_sample != kBitsPerSample)
            {
                throw std::runtime_error(
                    "Unsupported WAV format: expected PCM mono 44100 Hz 16-bit audio");
            }
        }

        std::vector<Waveform::Sample> ReadSamples(std::istream &input,
                                                  std::uint32_t chunk_size)
        {
            if (chunk_size % sizeof(Waveform::Sample) != 0)
            {
                throw std::runtime_error("Invalid WAV data chunk size");
            }

            std::vector<Waveform::Sample> samples(chunk_size / sizeof(Waveform::Sample));
            for (auto &sample : samples)
            {
                sample = ReadI16(input, "sample");
            }
            return samples;
        }

        void WriteExact(std::ostream &output, const char *data, std::streamsize size,
                        std::string_view field_name)
        {
            output.write(data, size);
            if (!output)
            {
                throw std::runtime_error("Failed to write WAV " + std::string(field_name));
            }
        }

        void WriteTag(std::ostream &output, std::string_view tag)
        {
            if (tag.size() != 4)
            {
                throw std::logic_error("WAV tag must be exactly 4 bytes");
            }
            WriteExact(output, tag.data(), static_cast<std::streamsize>(tag.size()), "tag");
        }

        void WriteU16(std::ostream &output, std::uint16_t value,
                      std::string_view field_name)
        {
            const std::array<unsigned char, 2> bytes{
                static_cast<unsigned char>(value & 0xFF),
                static_cast<unsigned char>((value >> 8) & 0xFF),
            };
            WriteExact(output, reinterpret_cast<const char *>(bytes.data()),
                       static_cast<std::streamsize>(bytes.size()), field_name);
        }

        void WriteU32(std::ostream &output, std::uint32_t value,
                      std::string_view field_name)
        {
            const std::array<unsigned char, 4> bytes{
                static_cast<unsigned char>(value & 0xFF),
                static_cast<unsigned char>((value >> 8) & 0xFF),
                static_cast<unsigned char>((value >> 16) & 0xFF),
                static_cast<unsigned char>((value >> 24) & 0xFF),
            };
            WriteExact(output, reinterpret_cast<const char *>(bytes.data()),
                       static_cast<std::streamsize>(bytes.size()), field_name);
        }

        void WriteI16(std::ostream &output, Waveform::Sample value)
        {
            WriteU16(output, static_cast<std::uint16_t>(value), "sample");
        }

    } // namespace

    Waveform WavReader::read(const std::string &file_name) const
    {
        std::ifstream input(file_name, std::ios::binary);
        if (!input)
        {
            throw std::runtime_error("Cannot open WAV input file: " + file_name);
        }

        if (!TagEquals(ReadTag(input, "RIFF tag"), "RIFF"))
        {
            throw std::runtime_error("Invalid WAV file: missing RIFF tag");
        }
        const std::uint32_t riff_size = ReadU32(input, "RIFF size");
        static_cast<void>(riff_size);
        if (!TagEquals(ReadTag(input, "WAVE tag"), "WAVE"))
        {
            throw std::runtime_error("Invalid WAV file: missing WAVE tag");
        }

        bool has_format = false;
        bool has_data = false;
        FormatChunk format{};
        std::vector<Waveform::Sample> samples;

        while (input.peek() != std::char_traits<char>::eof())
        {
            std::array<char, 4> chunk_tag{};
            if (!TryReadTag(input, chunk_tag))
            {
                break;
            }
            const std::uint32_t chunk_size = ReadU32(input, "chunk size");

            if (TagEquals(chunk_tag, "fmt "))
            {
                format = ReadFormatChunk(input, chunk_size);
                has_format = true;
            }
            else if (TagEquals(chunk_tag, "data"))
            {
                samples = ReadSamples(input, chunk_size);
                has_data = true;
            }
            else
            {
                SkipBytes(input, chunk_size);
            }

            if (chunk_size % 2 != 0)
            {
                SkipBytes(input, 1);
            }
        }

        if (!has_format)
        {
            throw std::runtime_error("Invalid WAV file: missing format chunk");
        }
        ValidateFormat(format);

        if (!has_data)
        {
            throw std::runtime_error("Invalid WAV file: missing data chunk");
        }

        return Waveform(std::move(samples), format.sample_rate);
    }

    void WavWriter::write(const std::string &file_name, const Waveform &waveform) const
    {
        const std::uint64_t data_size =
            static_cast<std::uint64_t>(waveform.size()) * sizeof(Waveform::Sample);
        const std::uint64_t riff_size = 36 + data_size;
        if (data_size > std::numeric_limits<std::uint32_t>::max() ||
            riff_size > std::numeric_limits<std::uint32_t>::max())
        {
            throw std::runtime_error("Waveform is too large to be written as WAV");
        }

        if (waveform.sampleRate() != Waveform::kDefaultSampleRate)
        {
            throw std::runtime_error("Unsupported waveform sample rate for WAV output");
        }

        std::ofstream output(file_name, std::ios::binary);
        if (!output)
        {
            throw std::runtime_error("Cannot open WAV output file: " + file_name);
        }

        const auto sample_rate = Waveform::kDefaultSampleRate;
        const auto byte_rate = sample_rate * kBlockAlign;

        WriteTag(output, "RIFF");
        WriteU32(output, static_cast<std::uint32_t>(riff_size), "RIFF size");
        WriteTag(output, "WAVE");

        WriteTag(output, "fmt ");
        WriteU32(output, 16, "format chunk size");
        WriteU16(output, kPcmFormat, "audio format");
        WriteU16(output, kMonoChannels, "channel count");
        WriteU32(output, sample_rate, "sample rate");
        WriteU32(output, byte_rate, "byte rate");
        WriteU16(output, kBlockAlign, "block align");
        WriteU16(output, kBitsPerSample, "bits per sample");

        WriteTag(output, "data");
        WriteU32(output, static_cast<std::uint32_t>(data_size), "data chunk size");
        for (const auto sample : waveform.samples())
        {
            WriteI16(output, sample);
        }
    }

} // namespace sound_processor
