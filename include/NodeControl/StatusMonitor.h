#pragma once
#include "NodeControl/Events.h"

namespace node
{

    class StatusMonitor
    {
    public:
        StatusMonitor() = default;
        void begin();
        void update();
        bool hasEvent() const;
        Event getEvent();

        private:
        static constexpr unsigned long MAX_INTERVAL = 5000;
        unsigned long m_lastUpdateTime = 0;
        EventEmitter m_eventEmitter;
    };
}