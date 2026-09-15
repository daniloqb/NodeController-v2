#include "communication/HeartBeatManager.h"
#include <Arduino.h>


namespace node
{
    HeartBeatManager::HeartBeatManager()
        : m_lastHeartbeatTime(0),
          m_waitingHeartbeat(false),
          m_event{},
          m_hasEvent(false)
    {
    }

    void HeartBeatManager::begin()
    {
        m_lastHeartbeatTime = millis();

        m_waitingHeartbeat = false;

        m_event = {};
        m_hasEvent = false;
    }

void HeartBeatManager::update()
{
    unsigned long currentTime = millis();

    if (currentTime - m_lastHeartbeatTime < HEARTBEAT_INTERVAL)
        return;

    m_lastHeartbeatTime = currentTime;

    if (m_waitingHeartbeat)
    {
        // Estado interno do HeartbeatManager
        m_waitingHeartbeat = false;

        // Comunica o ocorrido ao restante do sistema
        emitEvent(EventType::EVENT_HB_TIMEOUT);

        return;
    }

    m_waitingHeartbeat = true;

    emitEvent(EventType::EVENT_HB_REQUEST);
}

   void HeartBeatManager::handleEvent(const Event& event)
{
    switch (event.type)
    {
        case EventType::EVENT_HB_ACK:
            m_waitingHeartbeat = false;
            m_lastHeartbeatTime = millis();
            break;

        default:
            break;
    }
}

    bool HeartBeatManager::hasEvent() const
    {
        return m_hasEvent;
    }

    Event HeartBeatManager::getEvent()
    {
        Event event = m_event;

        m_event = {};
        m_hasEvent = false;

        return event;
    }

    void HeartBeatManager::emitEvent(EventType type)
    {
        // Evita sobrescrever um evento ainda não consumido.
        if (m_hasEvent)
            return;

        m_event.type = type;
        m_hasEvent = true;
    }
}