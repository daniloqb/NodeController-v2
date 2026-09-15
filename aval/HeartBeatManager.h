#pragma once
#include "core/Events.h"
namespace node
{

    class HeartBeatManager
    {
    public:
        HeartBeatManager();
        void begin();
        void update();
        void handleEvent(const Event &event);
        bool hasEvent() const;
        Event getEvent();

    private:
    void emitEvent(EventType type);

    private:
        unsigned long m_lastHeartbeatTime;

        bool m_waitingHeartbeat;

        Event m_event;
        bool m_hasEvent;

        static constexpr unsigned long HEARTBEAT_INTERVAL = 10000;
    };
}