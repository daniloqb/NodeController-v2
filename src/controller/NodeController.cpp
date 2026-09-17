#include "NodeController.h"
#include "nodes/LedNode.h"
#include <Arduino.h>

namespace node
{

    void node::NodeController::begin()
    {

        // Initialize all nodes here if needed

        LedNode *ledNode = new LedNode(13);
        addNode(*ledNode);

        for (size_t i = 0; i < m_nodeCount; ++i)
        {
            m_nodes[i]->begin();
        }   
    }

    void node::NodeController::update()
    {
        for (size_t i = 0; i < m_nodeCount; ++i)
        {
            m_nodes[i]->update();
        }           
    }

    void node::NodeController::handleEvent(const Event &event)
    {
        switch (event.type)
        {
        case EventType::EVENT_CMD_RCV:
            handleCommand(event.command);
            break;
        case EventType::EVENT_REBOOT:
            // Handle reboot event here
            break;

        default:
            break;
        }
    }

    bool node::NodeController::hasEvent() const
    {
        return m_eventEmitter.hasEvent();
    }

    Event node::NodeController::getEvent()
    {
        return m_eventEmitter.getEvent();
    }

    void NodeController::addNode(INode &node)
    {
        if (m_nodeCount < MAX_NODES)
        {
            m_nodes[m_nodeCount++] = &node;
        }   
    }

    void NodeController::handleCommand(const Command &command)
    {
        bool handled = false;
        for (size_t i = 0; i < m_nodeCount; ++i)
        {
            if (m_nodes[i]->accepts(command))
            {
                m_nodes[i]->handleCommand(command);
                handled = true;
                
            }
        }
        if (!handled)
        {
            // Handle unhandled command here
        }
    }

}