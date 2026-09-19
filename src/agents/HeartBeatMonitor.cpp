#include <NodeControl/HeartBeatMonitor.h>
#include <Arduino.h>

namespace node
{

    void HeartBeatMonitor::begin()
    {
        m_lastHeartbeatTime = millis();

        m_waitingHeartbeat = false;

        m_eventEmitter = EventEmitter();
    }

    void HeartBeatMonitor::update()
    {
        if (!m_run)
            return;
            
        unsigned long currentTime = millis();

        if (currentTime - m_lastHeartbeatTime < HEARTBEAT_INTERVAL)
            return;

        m_lastHeartbeatTime = currentTime;

        if (m_waitingHeartbeat)
        {
            // Estado interno do HeartbeatManager
            m_waitingHeartbeat = false;

            // Comunica o ocorrido ao restante do sistema
            m_eventEmitter.emitEvent(EventType::EVENT_HB_TIMEOUT);

            return;
        }

        m_waitingHeartbeat = true;

        m_eventEmitter.emitEvent(EventType::EVENT_HB_REQUEST);
    }

    void HeartBeatMonitor::handleEvent(const Event &event)
    {
        switch (event.type)
        {
        case EventType::EVENT_HB_ACK:
        case EventType::EVENT_REBOOT:
            m_waitingHeartbeat = false;
            m_lastHeartbeatTime = millis();
            break;

        default:
            break;
        }
    }

    bool HeartBeatMonitor::hasEvent() const
    {
        return m_eventEmitter.hasEvent();
    }
    Event HeartBeatMonitor::getEvent()
    {
        return m_eventEmitter.getEvent();
    }
    bool HeartBeatMonitor::disableHeartbeat()
    {
        m_run = false;
        return true;
    }

    bool HeartBeatMonitor::enableHeartbeat()
    {
        m_run = true;
        return true;
    }
}