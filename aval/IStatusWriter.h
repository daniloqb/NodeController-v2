#pragma once

#include "NodeControl/Value.h"

namespace node
{
    class IStatusWriter
    {
    public:
        virtual ~IStatusWriter() = default;

        virtual void begin() = 0;

        virtual void beginNode(
            char nodeId
        ) = 0;

        virtual void writeProperty(  char propertyId, const Value& value) = 0;

        virtual void endNode() = 0;

        virtual void end() = 0;
    };
}