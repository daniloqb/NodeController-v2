#pragma once

#include <stdint.h>

namespace node
{

    enum class ValueType : uint8_t
    {
        NONE,
        BOOLEAN,
        INTEGER,
        FLOAT,
    };

    union ValueData
    {
        bool booleanValue;
        int32_t integerValue;
        float floatValue;
    };

    struct Value
    {
        ValueType type = ValueType::NONE;
        ValueData data{};
    };

    inline Value makeBooleanValue(bool booleanValue)
    {
        Value value;
        value.type = ValueType::BOOLEAN;
        value.data.booleanValue = booleanValue;
        return value;
    }

    inline Value makeIntegerValue(int32_t integerValue)
    {
        Value value;
        value.type = ValueType::INTEGER;
        value.data.integerValue = integerValue;
        return value;
    }

    inline Value makeFloatValue(float floatValue)
    {
        Value value;
        value.type = ValueType::FLOAT;
        value.data.floatValue = floatValue;
        return value;
    }

}