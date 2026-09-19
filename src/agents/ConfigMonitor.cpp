#include <NodeControl/ConfigMonitor.h>
#include <Arduino.h>

namespace node
{
    ConfigMonitor::ConfigMonitor(StartupMode startupMode) : m_startupMode(startupMode)
    {
    }

    void ConfigMonitor::begin()
    {
        m_lastConfigTime = millis();
        m_waitingConfig = false;
        m_eventEmitter = EventEmitter();
        m_state = State::STATE_UP;
        m_run = (m_startupMode == StartupMode::MANAGED);
    }

    bool ConfigMonitor::isRunning() const
    {
        return m_run;
    }

    void ConfigMonitor::restartHandshake()
    {
        if (m_startupMode == StartupMode::MANAGED)
        {

            m_run = true;

            m_lastConfigTime = millis();
            m_waitingConfig = false;
            m_state = State::STATE_UP;
        }
    }

    void ConfigMonitor::stopHandshake()
    {
        m_run = false;
        m_state = State::STATE_UP;
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
                    m_eventEmitter.emitEvent(EventType::EVENT_CFG_TIMEOUT);
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
                m_eventEmitter.emitEvent(EventType::EVENT_CFG_REQUEST);
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
            if (m_state == State::STATE_CFG)
            {
                m_waitingConfig = false;
                m_lastConfigTime = 0;
                stopHandshake();
            }
            break;

        case EventType::EVENT_UP_ACK:

            if (m_state == State::STATE_UP)
            {
                m_waitingConfig = false;
                m_lastConfigTime = millis();
                m_state = State::STATE_CFG;
            }
            break;

        case EventType::EVENT_REBOOT:
        case EventType::EVENT_HB_TIMEOUT:
            restartHandshake();
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

    void ConfigMonitor::setStartupMode(StartupMode startupMode)
    {
        m_startupMode = startupMode;
    }

}