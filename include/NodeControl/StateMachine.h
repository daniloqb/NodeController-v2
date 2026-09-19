#pragma once
#include "NodeControl/Events.h"

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
            StateMachine() = default;
            void begin();
            void update();

            State getState() const;

            void handleEvent(const Event& event);

        private:
            State m_state = State::STATE_BOOT;

            void transitionTo(State newState);
    };

}