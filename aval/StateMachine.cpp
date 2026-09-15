#include "StateMachine.h"

namespace node
{

    StateMachine::StateMachine() : m_state(State::UP),
                                   m_stateChanged(false)
    {
    }

    void StateMachine::begin()
    {
        m_state = State::UP;
        m_stateChanged = false;
    }

    void StateMachine::update()
    {
        // This function can be used to perform periodic checks or updates
        // For now, it does nothing
    }

    

    void StateMachine::transitionTo(State newState)
    {
        if (m_state != newState)
        {
            m_state = newState;
            m_stateChanged = true;
        }
    }

    State StateMachine::getState() const
    {
        return m_state;
    }

    void StateMachine::clearStateChanged()
    {
        m_stateChanged = false;
    }

    bool StateMachine::isUp() const
    {
        return m_state == State::UP;
    }
    bool StateMachine::isCfg() const
    {
        return m_state == State::CFG;
    }
    bool StateMachine::isRunning() const
    {
        return m_state == State::RUN;
    }
    bool StateMachine::isIdle() const
    {
        return m_state == State::IDLE;
    }

    void node::StateMachine::handleEvent(const Event &event)
    {
        switch (m_state)
        {
        case State::UP:
            if (event.type == EventType::EVENT_ACK_UP)
            {
                transitionTo(State::CFG);
            }
            break;
        case State::CFG:
            if (event.type == EventType::EVENT_ACK_CFG)
            {
                transitionTo(State::RUN);
            }
            break;
        case State::RUN:
            if (event.type == EventType::EVENT_IDLE_RCV)
            {
                transitionTo(State::IDLE);
            }
            break;
        case State::IDLE:
            if (event.type == EventType::EVENT_RUN_RCV  )
            {
                transitionTo(State::RUN);
            }
            break;

        default:
            break;
        }

        if (event.type == EventType::EVENT_REBOOT)
        {
            transitionTo(State::UP);
            return;
        }
        if (event.type == EventType::EVENT_HB_TIMEOUT)
        {
            transitionTo(State::UP);
            return;
        }
    }
}