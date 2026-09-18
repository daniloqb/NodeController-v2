#include "NodeController.h"
#include "nodes/LedNode.h"


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

    CommandResult NodeController::handleCommand(const Command &command)
    {
        bool found = false;
        CommandResult finalResult = {};



        for (size_t i = 0; i < m_nodeCount; ++i)
        {
            INode *node = m_nodes[i];

            if (!node->accepts(command))
            {
                continue;
            }

            found = true;

            CommandResult result = node->handleCommand(command);

            if (result.type == CommandResultType::ERROR)
            {
                return result;
            }

            finalResult = result;
        }

        if (!found)
        {
            finalResult.type = CommandResultType::ERROR;
            finalResult.error = CommandError::NODE_NOT_FOUND;
        }

        return finalResult;
    }

    bool NodeController::pollEvent(NodeEvent &node)  {
        if (m_nodeCount == 0)
            return false;

        for (size_t offset = 0; offset < m_nodeCount; ++offset)
        {
            size_t i = (m_nextPollIndex + offset) % m_nodeCount;
            if (m_nodes[i]->pollEvent(node))
            {
                m_nextPollIndex = (i + 1) % m_nodeCount;
                return true;
            }
        }

        return false;
    }

}