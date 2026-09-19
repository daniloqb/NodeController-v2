#pragma once
#include "NodeControl/NodeEvent.h"
#include "NodeControl/Commands.h"
#include "NodeControl/IStatusWriter.h"

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