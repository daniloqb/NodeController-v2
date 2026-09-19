
#pragma once

#include "NodeControl/Orquestrator.h"
#include "NodeControl/ITransport.h"
#include "NodeControl/INode.h"
#include "NodeControl/StartupMode.h"

namespace node
{

class NodeDevice
{
public:
    explicit NodeDevice(ITransport& transport, const char* deviceConfig, StartupMode startupMode = StartupMode::MANAGED);

    void begin();
    void update();

    bool addNode(INode& node);

private:
    Orquestrator m_orchestrator;
    StartupMode m_startupMode = StartupMode::MANAGED;
};

}
