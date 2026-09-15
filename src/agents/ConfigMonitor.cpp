#include "agents/ConfigMonitor.h"
#include <Arduino.h>

namespace node
{

    void ConfigMonitor::begin()
    {
        m_lastConfigTime = millis();
        m_waitingConfig = false;
        m_eventEmitter = EventEmitter();
        m_state = State::STATE_UP;
    }

    bool ConfigMonitor::isRunning() const
    {
        return m_run;
    }

    void ConfigMonitor::startConfig()
    {
        m_run = true;

        m_lastConfigTime = millis();
        m_waitingConfig = false;
        m_state = State::STATE_UP;
    }

    void ConfigMonitor::stopConfig()
    {
        m_run = false;
    }

    void ConfigMonitor::update()

    {
        if (m_run)
        {
            unsigned long currentTime = millis();

            if (currentTime - m_lastConfigTime < CONFIG_INTERVAL)
                return;

            m_lastConfigTime = currentTime;

            if (m_waitingConfig)
            {
                // Estado interno do ConfigMonitor
                m_waitingConfig = false;

                // Comunica o ocorrido ao restante do sistema
                switch (m_state)
                {
                case State::STATE_UP:
                    m_eventEmitter.emitEvent(EventType::EVENT_UP_TIMEOUT);
                    break;
                case State::STATE_CFG:
                    m_eventEmitter.emitEvent(EventType::EVENT_CONFIG_TIMEOUT);
                    break;
                default:
                    break;
                }

                return;
            }

            m_waitingConfig = true;

            switch (m_state)
            {
            case State::STATE_UP:
                m_eventEmitter.emitEvent(EventType::EVENT_UP_REQUEST);
                break;
            case State::STATE_CFG:
                m_eventEmitter.emitEvent(EventType::EVENT_CONFIG_REQUEST);
                break;
            default:
                break;
            }
        }
    }

    void ConfigMonitor::handleEvent(const Event &event)
    {
        switch (event.type)
        {
        case EventType::EVENT_CFG_ACK:
            m_waitingConfig = false;
            m_lastConfigTime = 0;
            stopConfig();
            break;

        case EventType::EVENT_UP_ACK:
            m_waitingConfig = false;
            m_lastConfigTime = millis();
            m_state = State::STATE_CFG;
            break;

        case EventType::EVENT_REBOOT:
        case EventType::EVENT_HB_TIMEOUT:
            startConfig();
            break;

        default:
            break;
        }
    }

    bool ConfigMonitor::hasEvent() const

    {
        return m_eventEmitter.hasEvent();
    }
    Event ConfigMonitor::getEvent()
    {
        return m_eventEmitter.getEvent();
    }

}