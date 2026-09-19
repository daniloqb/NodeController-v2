#include <NodeControl/NodeDevice.h>

namespace node
{

NodeDevice::NodeDevice(ITransport& transport)
    : m_orchestrator(transport)
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