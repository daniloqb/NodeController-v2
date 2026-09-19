#include <NodeControl/StateMachine.h>

namespace node
{

     void StateMachine::begin()
    {
        m_state = State::STATE_UP;
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
        }
    }

    State StateMachine::getState() const
    {
        return m_state;
    }

    
    void node::StateMachine::handleEvent(const Event &event)
    {
        switch (m_state)
        {
        case State::STATE_UP:
            if (event.type == EventType::EVENT_UP_ACK)
            {
                transitionTo(State::STATE_CFG);
            }
            break;
        case State::STATE_CFG:
            if (event.type == EventType::EVENT_CFG_ACK)
            {
                transitionTo(State::STATE_RUN);
            }
            break;
        case State::STATE_RUN:
            if (event.type == EventType::EVENT_IDLE_RCV)
            {
                transitionTo(State::STATE_IDLE);
            } else {
                 if (event.type == EventType::EVENT_DIS_RCV)
            {
                transitionTo(State::STATE_DISABLED);
            } 
            }
            break;
        case State::STATE_IDLE:
        case State::STATE_DISABLED:
            if (event.type == EventType::EVENT_RUN_RCV  )
            {
                transitionTo(State::STATE_RUN);
            }
            break;

        default:
            break;
        }

        if (event.type == EventType::EVENT_REBOOT)
        {
            transitionTo(State::STATE_UP);
            return;
        }
        if (event.type == EventType::EVENT_HB_TIMEOUT)
        {
            transitionTo(State::STATE_UP);
            return;
        }
             if (event.type == EventType::EVENT_UP_TIMEOUT)
        {
            transitionTo(State::STATE_UP);
            return;
        }
             if (event.type == EventType::EVENT_CFG_TIMEOUT)
        {
            transitionTo(State::STATE_UP);
            return;
        }
    }
}