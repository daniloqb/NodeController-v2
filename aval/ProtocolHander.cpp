#include "protocol/ProtocolHandler.h"
#include <string.h>
#include <stdlib.h>

namespace node
{

    ProtocolHandler::ProtocolHandler() : m_index(0), m_hasEvent(false), m_event{}
    {
        clearBuffer();
    }

    void ProtocolHandler::clearBuffer()
    {
        memset(m_buffer, 0, BUFFER_SIZE);
        m_index = 0;
    }

    void ProtocolHandler::begin()
    {
        clearBuffer();
        m_hasEvent = false;
        m_event = {};
    }

    bool ProtocolHandler::hasEvent() const
    {
        return m_hasEvent;
    }

    Event ProtocolHandler::getEvent()
    {
        Event event = m_event;

        m_event = {};
        m_hasEvent = false;

        return event;
    }
    void ProtocolHandler::update(ITransport& transport)
    {
        while (transport.available())
        {
            uint8_t byte = transport.read();
            processByte(byte);
            if (m_hasEvent)
                return;
        }
    }
    void ProtocolHandler::processByte(uint8_t byte)
    {
        if (byte == '\r')
        {
            return;
        }
        if (byte == '\n')
        {

            if (m_index > 0)
            {
                m_buffer[m_index] = '\0';
                processMessage();
                clearBuffer();
            }
            return;
        }

        if (m_index < BUFFER_SIZE - 1)
        {
            m_buffer[m_index++] = static_cast<uint8_t>(byte);
        }
        else
        {
            clearBuffer();
        }
    }

    void ProtocolHandler::processMessage()
    {

        if (strcmp(reinterpret_cast<char *>(m_buffer), "ACK_UP") == 0)
        {
            m_event.type = EventType::EVENT_ACK_UP;
            m_hasEvent = true;
            return;
        }
        if (strcmp(reinterpret_cast<char *>(m_buffer), "ACK_CFG") == 0)
        {
            m_event.type = EventType::EVENT_ACK_CFG;
            m_hasEvent = true;
            return;
        }
        if (strcmp(reinterpret_cast<char *>(m_buffer), "ACK_HB") == 0)
        {
            m_event.type = EventType::EVENT_HB_ACK;
            m_hasEvent = true;
            return;
        }
        if (strcmp(reinterpret_cast<char *>(m_buffer), "ACK_CMD") == 0)
        {
            m_event.type = EventType::EVENT_ACK_CMD;
            m_hasEvent = true;
            return;
        }
        if (strncmp(m_buffer, "CMD:", 4) == 0)
        {
            m_event.type = EventType::EVENT_CMD_RCV;

            m_event.nodeId =
                static_cast<uint8_t>(atoi(m_buffer + 4));

            m_hasEvent = true;
            return;
        }
        if (strcmp(reinterpret_cast<char *>(m_buffer), "RUN") == 0)
        {
            m_event.type = EventType::EVENT_RUN_RCV;
            m_hasEvent = true;
            return;
        }
        if (strcmp(reinterpret_cast<char *>(m_buffer), "IDLE") == 0)
        {
            m_event.type = EventType::EVENT_IDLE_RCV;
            m_hasEvent = true;
            return;
        }
    }
    void node::ProtocolHandler::sendHeartbeat(ITransport &transport)
    {
        const char message[] = "HB\n";

        transport.write(
            reinterpret_cast<const uint8_t *>(message),
            sizeof(message) - 1);
    }
}