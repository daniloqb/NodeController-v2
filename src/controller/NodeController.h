#pragma once

#include "core/Events.h"
#include "core/Commands.h"
#include "nodes/INode.h"

namespace node
{
    class NodeController
    {
    public:
        NodeController() = default;
        void begin();
        void update();
     
        CommandResult handleCommand(const Command &command);

        void handleEvent(const Event &event);
        bool pollEvent(NodeEvent& node);
        bool hasEvent() const;
        Event getEvent();

        void addNode(INode& node);
    private:
        EventEmitter m_eventEmitter;
        
    private:
        static constexpr size_t MAX_NODES = 2;
        INode* m_nodes[MAX_NODES] = {};
        size_t m_nodeCount = 0;
        size_t m_nextPollIndex = 0;
    };
}