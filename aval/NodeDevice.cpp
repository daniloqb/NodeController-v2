#include <NodeControl/NodeDevice.h>

namespace node
{

NodeDevice::NodeDevice(ITransport& transport, const char* deviceConfig)
    : m_orchestrator(transport, deviceConfig)
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