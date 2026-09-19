#pragma once
#include "NodeControl/Events.h"
#include "NodeControl/StateMachine.h"



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
        void restartHandshake();
        void stopHandshake();

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