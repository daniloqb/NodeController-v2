#pragma once
#include "NodeControl/Events.h"
#include "NodeControl/StartupMode.h"

namespace node{

       enum class State{
        STATE_BOOT,
        STATE_UP,
        STATE_CFG,
        STATE_RUN,
        STATE_IDLE,
        STATE_DISABLED,
    };

    class StateMachine{
        public:
            StateMachine(StartupMode startupMode = StartupMode::MANAGED);
            void begin();
            void update();
            void reset();
            State getState() const;

            void handleEvent(const Event& event);
            void setStartupMode(StartupMode startupMode);

        private:
            State m_state = State::STATE_BOOT;
            StartupMode m_startupMode = StartupMode::MANAGED;

            void transitionTo(State newState);
    };

}