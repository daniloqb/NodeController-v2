#pragma once
#include <stdint.h>

namespace node
{
    enum class EventType
    {
        EVENT_NONE,
        EVENT_UP_ACK,
        EVENT_CONFIG_REQUEST,
        EVENT_CONFIG_TIMEOUT,
        EVENT_UP_REQUEST,
        EVENT_UP_TIMEOUT,
        EVENT_RUN_RCV,
        EVENT_IDLE_RCV,
        EVENT_CFG_ACK,
        EVENT_CFG_TIMEOUT,
        EVENT_DIS_RCV,
        EVENT_CMD_ACK,
        EVENT_CMD_RCV,
        EVENT_CMD_TIMEOUT,
        EVENT_HB_ACK,
        EVENT_HB_REQUEST,
        EVENT_HB_TIMEOUT,
        EVENT_REBOOT,
        EVENT_GET_STATE,
    };

struct Event
{
    EventType type = EventType::EVENT_NONE;
    uint8_t nodeId = 0;
    int data = 0;
   
};

class EventEmitter{
    public:
        EventEmitter() = default;

        void emitEvent(EventType type)
        {
            // Evita sobrescrever um evento ainda não consumido.
            if (m_hasEvent)
                return;

            m_event.type = type;
            m_hasEvent = true;
        }

        bool hasEvent() const
        {
            return m_hasEvent;
        }

        Event getEvent()
        {
            Event event = m_event;
            m_event = {};
            m_hasEvent = false;
            return event;
        }

    private:
        Event m_event = {};
        bool m_hasEvent = false;
};      

}