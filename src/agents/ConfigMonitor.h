#pragma once
#include "core/Events.h"
#include "core/StateMachine.h"



namespace node
{

    class ConfigMonitor
    {
    public:
        ConfigMonitor() = default;
        void begin();
        void update();
        void handleEvent(const Event &event);
        bool hasEvent() const;
        Event getEvent();
        bool isRunning() const;
        void startConfig();
        void stopConfig();

    private:


    private:
        State m_state = State::STATE_UP;
        EventEmitter m_eventEmitter;
        unsigned long m_lastConfigTime = 0;
        bool m_waitingConfig = false;
        bool m_run = false;
        static constexpr unsigned long CONFIG_INTERVAL = 2000;
    };
}