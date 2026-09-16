#pragma once

#include "core/Events.h"
#include "../nodes/LedNode.h"

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
    private:
        EventEmitter m_eventEmitter;
        void handleCommand(const Command &command);
        LedNode m_ledNode;
    };
}