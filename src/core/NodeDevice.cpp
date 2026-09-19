#include <NodeControl/NodeDevice.h>

namespace node
{


NodeDevice::NodeDevice(ITransport& transport, const char* deviceConfig, StartupMode startupMode)
    : m_orchestrator(transport, deviceConfig, startupMode), m_startupMode(startupMode)
{
}

void NodeDevice::begin()
{
    m_orchestrator.begin();
}

void NodeDevice::update()
{
    m_orchestrator.update();
}

bool NodeDevice::addNode(INode& node)
{
    m_orchestrator.addNode(node);
    return true;
}

}