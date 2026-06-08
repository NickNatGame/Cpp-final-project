#include "sound_processor/json/json_pipeline_loader.h"

#include <cctype>
#include <fstream>
#include <map>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace sound_processor
{
    namespace
    {
        struct JsonValue
        {
            enum class Type
            {
                nullValue,
                boolValue,
                numberValue,
                stringValue,
                arrayValue,
                objectValue
            };

            Type type = Type::nullValue;
            std::string scalar;
            std::vector<JsonValue> array;
            std::map<std::string, JsonValue> object;
        };

        class JsonParser
        {
        public:
            explicit JsonParser(std::string text) : text_(std::move(text))
            {
            }

            JsonValue parse()
            {
                JsonValue value = parseValue();
                skipWhitespace();
                if (!eof())
                {
                    fail("Unexpected trailing characters");
                }
                return value;
            }

        private:
            [[nodiscard]] bool eof() const noexcept
            {
                return pos_ >= text_.size();
            }

            [[nodiscard]] char peek() const
            {
                if (eof())
                {
                    fail("Unexpected end of JSON");
                }
                return text_[pos_];
            }

            char take()
            {
                const char result = peek();
                ++pos_;
                return result;
            }

            void expect(char expected)
            {
                if (take() != expected)
                {
                    fail(std::string("Expected '") + expected + "'");
                }
            }

            void skipWhitespace()
            {
                while (!eof() && std::isspace(static_cast<unsigned char>(text_[pos_])))
                {
                    ++pos_;
                }
            }

            JsonValue parseValue()
            {
                skipWhitespace();
                const char current = peek();
                if (current == '"')
                {
                    JsonValue value;
                    value.type = JsonValue::Type::stringValue;
                    value.scalar = parseString();
                    return value;
                }
                if (current == '{')
                {
                    return parseObject();
                }
                if (current == '[')
                {
                    return parseArray();
                }
                if (current == '-' || std::isdigit(static_cast<unsigned char>(current)))
                {
                    JsonValue value;
                    value.type = JsonValue::Type::numberValue;
                    value.scalar = parseNumber();
                    return value;
                }
                if (consumeLiteral("true"))
                {
                    JsonValue value;
                    value.type = JsonValue::Type::boolValue;
                    value.scalar = "true";
                    return value;
                }
                if (consumeLiteral("false"))
                {
                    JsonValue value;
                    value.type = JsonValue::Type::boolValue;
                    value.scalar = "false";
                    return value;
                }
                if (consumeLiteral("null"))
                {
                    return JsonValue{};
                }
                fail("Unexpected JSON token");
            }

            JsonValue parseObject()
            {
                JsonValue value;
                value.type = JsonValue::Type::objectValue;
                expect('{');
                skipWhitespace();
                if (peek() == '}')
                {
                    take();
                    return value;
                }

                while (true)
                {
                    skipWhitespace();
                    if (peek() != '"')
                    {
                        fail("Expected object key");
                    }
                    std::string key = parseString();
                    skipWhitespace();
                    expect(':');
                    value.object.emplace(std::move(key), parseValue());
                    skipWhitespace();
                    const char separator = take();
                    if (separator == '}')
                    {
                        return value;
                    }
                    if (separator != ',')
                    {
                        fail("Expected ',' or '}' in object");
                    }
                }
            }

            JsonValue parseArray()
            {
                JsonValue value;
                value.type = JsonValue::Type::arrayValue;
                expect('[');
                skipWhitespace();
                if (peek() == ']')
                {
                    take();
                    return value;
                }

                while (true)
                {
                    value.array.push_back(parseValue());
                    skipWhitespace();
                    const char separator = take();
                    if (separator == ']')
                    {
                        return value;
                    }
                    if (separator != ',')
                    {
                        fail("Expected ',' or ']' in array");
                    }
                }
            }

            std::string parseString()
            {
                expect('"');
                std::string result;
                while (true)
                {
                    if (eof())
                    {
                        fail("Unterminated string");
                    }
                    const char current = take();
                    if (current == '"')
                    {
                        return result;
                    }
                    if (current == '\\')
                    {
                        if (eof())
                        {
                            fail("Unterminated escape sequence");
                        }
                        const char escaped = take();
                        switch (escaped)
                        {
                        case '"':
                        case '\\':
                        case '/':
                            result.push_back(escaped);
                            break;
                        case 'n':
                            result.push_back('\n');
                            break;
                        case 'r':
                            result.push_back('\r');
                            break;
                        case 't':
                            result.push_back('\t');
                            break;
                        default:
                            fail("Unsupported escape sequence");
                        }
                    }
                    else
                    {
                        result.push_back(current);
                    }
                }
            }

            std::string parseNumber()
            {
                const std::size_t start = pos_;
                if (peek() == '-')
                {
                    ++pos_;
                }
                consumeDigits();
                if (!eof() && peek() == '.')
                {
                    ++pos_;
                    consumeDigits();
                }
                if (!eof() && (peek() == 'e' || peek() == 'E'))
                {
                    ++pos_;
                    if (!eof() && (peek() == '+' || peek() == '-'))
                    {
                        ++pos_;
                    }
                    consumeDigits();
                }
                return text_.substr(start, pos_ - start);
            }

            void consumeDigits()
            {
                if (eof() || !std::isdigit(static_cast<unsigned char>(peek())))
                {
                    fail("Expected digit in number");
                }
                while (!eof() && std::isdigit(static_cast<unsigned char>(text_[pos_])))
                {
                    ++pos_;
                }
            }

            bool consumeLiteral(const char *literal)
            {
                const std::string expected(literal);
                if (text_.compare(pos_, expected.size(), expected) == 0)
                {
                    pos_ += expected.size();
                    return true;
                }
                return false;
            }

            [[noreturn]] void fail(const std::string &message) const
            {
                throw std::runtime_error("JSON parse error at byte " + std::to_string(pos_) + ": " + message);
            }

            std::string text_;
            std::size_t pos_ = 0;
        };

        [[nodiscard]] std::string ReadTextFile(const std::string &file_name)
        {
            std::ifstream input(file_name);
            if (!input)
            {
                throw std::runtime_error("Cannot open JSON config: " + file_name);
            }
            std::ostringstream buffer;
            buffer << input.rdbuf();
            return buffer.str();
        }

        [[nodiscard]] const JsonValue *Find(const JsonValue &object, const std::string &key)
        {
            const auto it = object.object.find(key);
            return it == object.object.end() ? nullptr : &it->second;
        }

        [[nodiscard]] std::string ValueToToken(const JsonValue &value)
        {
            if (value.type == JsonValue::Type::stringValue || value.type == JsonValue::Type::numberValue ||
                value.type == JsonValue::Type::boolValue)
            {
                return value.scalar;
            }
            throw std::runtime_error("Filter params must be strings, numbers or booleans");
        }

        [[nodiscard]] std::string RequiredString(const JsonValue &object, const std::string &key)
        {
            const JsonValue *value = Find(object, key);
            if (value == nullptr || value->type != JsonValue::Type::stringValue)
            {
                throw std::runtime_error("JSON field '" + key + "' must be a string");
            }
            return value->scalar;
        }

        void ReadOptionalString(const JsonValue &object, const std::string &key, std::optional<std::string> &target)
        {
            const JsonValue *value = Find(object, key);
            if (value == nullptr || value->type == JsonValue::Type::nullValue)
            {
                return;
            }
            if (value->type != JsonValue::Type::stringValue)
            {
                throw std::runtime_error("JSON field '" + key + "' must be a string");
            }
            target = value->scalar;
        }

        [[nodiscard]] FilterDescriptor ReadFilter(const JsonValue &value)
        {
            if (value.type != JsonValue::Type::objectValue)
            {
                throw std::runtime_error("Every JSON filter entry must be an object");
            }

            FilterDescriptor descriptor;
            descriptor.name = RequiredString(value, "name");

            const JsonValue *params = Find(value, "params");
            if (params == nullptr || params->type == JsonValue::Type::nullValue)
            {
                return descriptor;
            }
            if (params->type != JsonValue::Type::arrayValue)
            {
                throw std::runtime_error("JSON filter params must be an array");
            }
            for (const auto &param : params->array)
            {
                descriptor.params.push_back(ValueToToken(param));
            }
            return descriptor;
        }
    } // namespace

    ParsedArgs JsonPipelineLoader::load(const std::string &file_name) const
    {
        JsonParser parser(ReadTextFile(file_name));
        const JsonValue root = parser.parse();
        if (root.type != JsonValue::Type::objectValue)
        {
            throw std::runtime_error("JSON config root must be an object");
        }

        ParsedArgs args;
        ReadOptionalString(root, "input", args.input_file);
        ReadOptionalString(root, "output", args.output_file);

        const JsonValue *filters = Find(root, "filters");
        if (filters == nullptr || filters->type == JsonValue::Type::nullValue)
        {
            return args;
        }
        if (filters->type != JsonValue::Type::arrayValue)
        {
            throw std::runtime_error("JSON field 'filters' must be an array");
        }
        for (const auto &filter : filters->array)
        {
            args.filters.push_back(ReadFilter(filter));
        }
        return args;
    }

} // namespace sound_processor
