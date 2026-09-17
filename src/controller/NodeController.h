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
        void handleEvent(const Event &event);

        bool hasEvent() const;
        Event getEvent();

        void addNode(INode& node);
    private:
        EventEmitter m_eventEmitter;
        void handleCommand(const Command &command);
        
    private:
        static constexpr size_t MAX_NODES = 8;
        INode* m_nodes[MAX_NODES] = {};
        size_t m_nodeCount = 0;
    };
}