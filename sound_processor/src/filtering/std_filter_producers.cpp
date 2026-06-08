#include "sound_processor/filtering/std_filter_producers.h"

#include "sound_processor/filters/ampl_filter.h"
#include "sound_processor/filters/lowpass_filter.h"
#include "sound_processor/filters/normalize_filter.h"
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
    } // namespace

    void RegisterTransformFilters(FilterRegistry &registry)
    {
        registry.add("ampl", CreateAmpl);
        registry.add("normalize", CreateNormalize);
        registry.add("silence", CreateSilence);
        registry.add("timestretch", CreateTimestretch);
        registry.add("lowpass", CreateLowpass);
    }

} // namespace sound_processor
