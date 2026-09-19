#pragma once
#include "NodeControl/Events.h"
namespace node
{

    class HeartBeatMonitor
    {
    public:
        HeartBeatMonitor() = default;
        void begin();
        void update();
        void handleEvent(const Event &event);
        bool hasEvent() const;
        Event getEvent();

    private:


    private:
        EventEmitter m_eventEmitter;
        unsigned long m_lastHeartbeatTime = 0;
        bool m_waitingHeartbeat = false;
        static constexpr unsigned long HEARTBEAT_INTERVAL = 60000;
    };
}