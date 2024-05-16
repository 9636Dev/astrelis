#pragma once

#include <sstream>
#include <utility>
#include <vector>
#include <string>

namespace Nebula::Profiling
{
    class JsonArray;
    class JsonObject;

    class JsonBase
    {
    public:
        JsonBase()                                                    = default;
        JsonBase(const JsonBase&)                                     = default;
        JsonBase(JsonBase&&)                                          = default;
        JsonBase& operator=(const JsonBase&)                          = default;
        JsonBase& operator=(JsonBase&&)                               = default;
        virtual ~JsonBase()                                           = default;
        virtual void AppendToStream(std::stringstream& sstream) const = 0;
        virtual std::string StringValue() const
        {
            std::stringstream sstream;
            AppendToStream(sstream);
            return sstream.str();
        };
    };

    class JsonValue : public JsonBase
    {
    public:
        explicit JsonValue(std::string value)
            : m_Value(std::move(value))
        {
        }
        explicit JsonValue(const JsonBase& value);

        ~JsonValue() override = default;
        JsonValue(const JsonValue&) = default;
        JsonValue(JsonValue&&) noexcept = default;
        JsonValue& operator=(const JsonValue&) = default;
        JsonValue& operator=(JsonValue&&) noexcept = default;

        void AppendToStream(std::stringstream& sstream) const override
        {
            sstream << m_Value;
        }
    private:
        std::string m_Value;
    };

    class JsonArray : public JsonBase
    {
    public:
        JsonArray() = default;
        ~JsonArray() override = default;
        JsonArray(const JsonArray&) = default;
        JsonArray(JsonArray&&) noexcept = default;
        JsonArray& operator=(const JsonArray&) = default;
        JsonArray& operator=(JsonArray&&) noexcept = default;

        void AppendToStream(std::stringstream& sstream) const override
        {
            sstream << "[";
            for (const auto& value : m_Values)
            {
                value.AppendToStream(sstream);
                sstream << ",";
            }
            if (!m_Values.empty())
            {
                sstream.seekp(-1, std::ios_base::end);
                sstream << "";
            }
            sstream << "]";
        }

        void AddValue(const JsonValue& value) { m_Values.push_back(value); }
        void AddValue(const JsonBase& value) { m_Values.emplace_back(value); }

    private:
        std::vector<JsonValue> m_Values;
    };

    class JsonObject : public JsonBase
    {
    public:
        JsonObject() = default;
        ~JsonObject() override = default;
        JsonObject(const JsonObject&) = default;
        JsonObject(JsonObject&&) noexcept = default;
        JsonObject& operator=(const JsonObject&) = default;
        JsonObject& operator=(JsonObject&&) noexcept = default;

        void AppendToStream(std::stringstream& sstream) const override
        {
            sstream << "{";
            for (const auto& [key, value] : m_Values)
            {
                sstream << "\"" << key << "\": ";
                value.AppendToStream(sstream);
                sstream << ",";
            }
            if (!m_Values.empty())
            {
                sstream.seekp(-1, std::ios_base::end);
                sstream << "";
            }
            sstream << "}";
        }

        void AddValue(const std::string& key, const JsonValue& value) { m_Values.emplace_back(key, value); }
        void AddValue(const std::string& key, const JsonBase& value) { m_Values.emplace_back(key, value); }

        [[nodiscard]] JsonValue& operator[](const std::string& key)
        {
            for (auto& [k, v] : m_Values)
            {
                if (k == key)
                {
                    return v;
                }
            }
            m_Values.emplace_back(key, JsonValue(""));
            return m_Values.back().second;
        }

    private:
        std::vector<std::pair<std::string, JsonValue>> m_Values;
    };

    inline JsonValue::JsonValue(const JsonBase& value)
        : m_Value(value.StringValue())
    {
    }

}  // namespace Nebula::Profiling

