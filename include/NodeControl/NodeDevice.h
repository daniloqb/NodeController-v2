
#pragma once

#include "NodeControl/Orquestrator.h"
#include "NodeControl/ITransport.h"
#include "NodeControl/INode.h"

namespace node
{

class NodeDevice
{
public:
    explicit NodeDevice(ITransport& transport);

    void begin();
    void update();

    bool addNode(INode& node);

private:
    Orquestrator m_orchestrator;
};

}
