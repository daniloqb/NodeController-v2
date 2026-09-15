#pragma once
#include "core/Events.h"

namespace node{

       enum class State{
        UP,
        CFG,
        RUN,
        IDLE,
    };



    class StateMachine{
        public:
            StateMachine();
            void begin();
            void update();

            State getState() const;

            bool isUp() const;
            bool isCfg() const;
            bool isRunning() const;
            bool isIdle() const;

            void clearStateChanged();
            void handleEvent(const Event& event);

        private:
            State m_state;
            bool m_stateChanged;

            void transitionTo(State newState);
    };

}