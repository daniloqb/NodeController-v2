#pragma once
#include "NodeControl/ITransport.h"
#include "NodeControl/IStatusWriter.h"
#include "NodeControl/Value.h"

namespace node
{
    class JSONStatusWriter : public IStatusWriter
    {
    public:
        explicit JSONStatusWriter(ITransport &transport);
        void begin() override;
        void beginNode(char nodeId) override;
        void endNode() override;
        void end() override;
        void writeProperty(char propertyId, const Value &value) override;

    private:
        ITransport &m_transport;
        bool m_firstProperty = true;
        bool m_firstNode = true;

        void writeText(const char *text);
        void writeChar(char value);
        void writeValue(const Value &value);
    };
}