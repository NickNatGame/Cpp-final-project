#include "sound_processor/filtering/std_filter_producers.h"

#include "sound_processor/filters/ampl_filter.h"
#include "sound_processor/filters/generators/am_generator_filter.h"
#include "sound_processor/filters/generators/fm_generator_filter.h"
#include "sound_processor/filters/generators/sine_generator_filter.h"
#include "sound_processor/filters/fade_filter.h"
#include "sound_processor/filters/lowpass_filter.h"
#include "sound_processor/filters/normalize_filter.h"
#include "sound_processor/filters/reverse_filter.h"
#include "sound_processor/filters/silence_filter.h"
#include "sound_processor/filters/timestretch_filter.h"

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>

namespace sound_processor
{
    namespace
    {
        void RequireParamCount(const FilterDescriptor &descriptor, std::size_t min_count, std::size_t max_count)
        {
            if (descriptor.params.size() < min_count || descriptor.params.size() > max_count)
            {
                throw std::runtime_error("Wrong parameter count for filter: " + descriptor.name);
            }
        }

        double ParseDouble(const std::string &value, const std::string &param_name)
        {
            std::size_t parsed = 0;
            try
            {
                const double result = std::stod(value, &parsed);
                if (parsed != value.size())
                {
                    throw std::runtime_error("Trailing characters");
                }
                return result;
            }
            catch (const std::exception &)
            {
                throw std::runtime_error("Cannot parse " + param_name + " as floating point number: " + value);
            }
        }

        std::size_t ParseSize(const std::string &value, const std::string &param_name)
        {
            std::size_t parsed = 0;
            try
            {
                const unsigned long result = std::stoul(value, &parsed);
                if (parsed != value.size())
                {
                    throw std::runtime_error("Trailing characters");
                }
                return static_cast<std::size_t>(result);
            }
            catch (const std::exception &)
            {
                throw std::runtime_error("Cannot parse " + param_name + " as positive integer: " + value);
            }
        }

        double ConvertToSeconds(const std::string &unit, double value)
        {
            if (unit == "sec")
            {
                return value;
            }
            if (unit == "ms")
            {
                return value / 1000.0;
            }
            throw std::runtime_error("silence unit must be either sec or ms");
        }

        std::unique_ptr<IFilter> CreateAmpl(const FilterDescriptor &descriptor)
        {
            RequireParamCount(descriptor, 1, 1);
            return std::make_unique<AmplFilter>(ParseDouble(descriptor.params[0], "ampl factor"));
        }

        std::unique_ptr<IFilter> CreateNormalize(const FilterDescriptor &descriptor)
        {
            RequireParamCount(descriptor, 0, 1);
            const double peak = descriptor.params.empty() ? 1.0 : ParseDouble(descriptor.params[0], "normalize peak");
            return std::make_unique<NormalizeFilter>(peak);
        }

        std::unique_ptr<IFilter> CreateSilence(const FilterDescriptor &descriptor)
        {
            RequireParamCount(descriptor, 3, 3);
            const double start = ParseDouble(descriptor.params[1], "silence start");
            const double end = ParseDouble(descriptor.params[2], "silence end");
            return std::make_unique<SilenceFilter>(ConvertToSeconds(descriptor.params[0], start),
                                                   ConvertToSeconds(descriptor.params[0], end));
        }

        std::unique_ptr<IFilter> CreateTimestretch(const FilterDescriptor &descriptor)
        {
            RequireParamCount(descriptor, 1, 1);
            return std::make_unique<TimestretchFilter>(ParseDouble(descriptor.params[0], "timestretch factor"));
        }

        std::unique_ptr<IFilter> CreateLowpass(const FilterDescriptor &descriptor)
        {
            RequireParamCount(descriptor, 1, 1);
            return std::make_unique<LowpassFilter>(ParseSize(descriptor.params[0], "lowpass window_size"));
        }


        std::unique_ptr<IFilter> CreateGenerator(const FilterDescriptor &descriptor)
        {
            if (descriptor.params.empty())
            {
                throw std::runtime_error("generator requires signal type: sin, am or fm");
            }

            const std::string &type = descriptor.params[0];
            if (type == "sin")
            {
                RequireParamCount(descriptor, 3, 3);
                return std::make_unique<SineGeneratorFilter>(ParseDouble(descriptor.params[1], "generator sin frequency_hz"),
                                                             ParseDouble(descriptor.params[2], "generator sin duration_ms"));
            }
            if (type == "am")
            {
                RequireParamCount(descriptor, 6, 6);
                return std::make_unique<AmGeneratorFilter>(
                    ParseDouble(descriptor.params[1], "generator am amplitude"),
                    ParseDouble(descriptor.params[2], "generator am carrier_hz"),
                    ParseDouble(descriptor.params[3], "generator am modulation_hz"),
                    ParseDouble(descriptor.params[4], "generator am depth"),
                    ParseDouble(descriptor.params[5], "generator am duration_ms"));
            }
            if (type == "fm")
            {
                RequireParamCount(descriptor, 6, 6);
                return std::make_unique<FmGeneratorFilter>(
                    ParseDouble(descriptor.params[1], "generator fm amplitude"),
                    ParseDouble(descriptor.params[2], "generator fm carrier_hz"),
                    ParseDouble(descriptor.params[3], "generator fm modulation_hz"),
                    ParseDouble(descriptor.params[4], "generator fm deviation_hz"),
                    ParseDouble(descriptor.params[5], "generator fm duration_ms"));
            }

            throw std::runtime_error("Unknown generator type: " + type);
        }


        std::unique_ptr<IFilter> CreateFade(const FilterDescriptor &descriptor)
        {
            RequireParamCount(descriptor, 2, 2);
            FadeFilter::Mode mode;
            if (descriptor.params[0] == "in")
            {
                mode = FadeFilter::Mode::in;
            }
            else if (descriptor.params[0] == "out")
            {
                mode = FadeFilter::Mode::out;
            }
            else
            {
                throw std::runtime_error("fade mode must be either in or out");
            }
            return std::make_unique<FadeFilter>(mode, ParseDouble(descriptor.params[1], "fade duration_ms"));
        }

        std::unique_ptr<IFilter> CreateReverse(const FilterDescriptor &descriptor)
        {
            RequireParamCount(descriptor, 0, 0);
            return std::make_unique<ReverseFilter>();
        }
    } // namespace

    void RegisterTransformFilters(FilterRegistry &registry)
    {
        registry.add("ampl", CreateAmpl);
        registry.add("normalize", CreateNormalize);
        registry.add("silence", CreateSilence);
        registry.add("timestretch", CreateTimestretch);
        registry.add("lowpass", CreateLowpass);
        registry.add("generator", CreateGenerator);
        registry.add("fade", CreateFade);
        registry.add("reverse", CreateReverse);
    }

} // namespace sound_processor
