#pragma once
#include <stdint.h>
#include <stddef.h>
#include "core/Commands.h"

namespace node
{

      
    enum class EventType
    {
        EVENT_NONE,
        EVENT_UP_ACK,

        EVENT_UP_REQUEST,
        EVENT_UP_TIMEOUT,
        EVENT_RUN_RCV,
        EVENT_IDLE_RCV,
        EVENT_CFG_ACK,
        EVENT_CFG_REQUEST,
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
        EVENT_CMD_ERROR,
        EVENT_CMD_PARSE_ERROR,
    };

    struct Event
    {
        EventType type = EventType::EVENT_NONE;
        Command command = {};
    };

    class EventEmitter
    {
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

        void emitEvent(const Event &event)
        {
            // Evita sobrescrever um evento ainda não consumido.
            if (m_hasEvent)
                return;

            m_event = event;
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