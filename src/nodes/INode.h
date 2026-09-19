#pragma once
#include "core/NodeEvent.h"
#include "core/Commands.h"
#include "protocol/IStatusWriter.h"

namespace node
{
class INode
{
public:
    virtual ~INode() = default;

    virtual void begin() = 0;
    virtual void update() = 0;
    virtual void reset() = 0;
    
    virtual char getId() const = 0;

    virtual CommandResult handleCommand(const Command& command) = 0;

    virtual bool pollEvent(NodeEvent& event) = 0;

    virtual void writeStatus(IStatusWriter& statusWriter) const = 0;
};
}