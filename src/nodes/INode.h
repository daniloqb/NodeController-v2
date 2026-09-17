#pragma once
#include "core/NodeEvent.h"
#include "core/Commands.h"

namespace node
{
class INode
{
public:
    virtual ~INode() = default;

    virtual void begin() = 0;
    virtual void update() = 0;
    virtual void reset() = 0;
    
    virtual const char* getId() const = 0;
    virtual bool accepts(const Command& command) const = 0;

    virtual CommandResult handleCommand(const Command& command) = 0;

    virtual bool pollEvent(NodeEvent& event) = 0;
};
}