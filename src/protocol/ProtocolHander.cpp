#include "protocol/ProtocolHandler.h"
#include <string.h>
#include <stdlib.h>

namespace node
{

    void ProtocolHandler::begin()
    {
        clearBuffer();
        m_eventEmitter = EventEmitter();
    }

    void ProtocolHandler::clearBuffer()
    {
        memset(m_buffer, 0, BUFFER_SIZE);
        m_index = 0;
    }

    void ProtocolHandler::update(ITransport &transport)
    {
        while (transport.available())
        {
            uint8_t byte = transport.read();
            receive(byte);
            if (m_eventEmitter.hasEvent())
                return;
        }
    }
    void ProtocolHandler::receive(uint8_t byte)
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

    void ProtocolHandler::receiveMessage(const char *message)
    {
        clearBuffer();

        while (*message && m_index < BUFFER_SIZE - 1)
        {
            m_buffer[m_index++] = *message++;
        }

        m_buffer[m_index] = '\0';

        processMessage();

        clearBuffer();
    }

    void node::ProtocolHandler::processMessage()
    {
        ProtocolMessage message =
            stringToProtocolMessage(m_buffer);

        switch (message)
        {
        case ProtocolMessage::MESSAGE_UP_ACK:

            m_eventEmitter.emitEvent(
                EventType::EVENT_UP_ACK);

            break;

        case ProtocolMessage::MESSAGE_CFG_ACK:

            m_eventEmitter.emitEvent(
                EventType::EVENT_CFG_ACK);

            break;

        case ProtocolMessage::MESSAGE_HB_ACK:

            m_eventEmitter.emitEvent(
                EventType::EVENT_HB_ACK);

            break;

        case ProtocolMessage::MESSAGE_CMD_ACK:

            m_eventEmitter.emitEvent(
                EventType::EVENT_CMD_ACK);

            break;

        case ProtocolMessage::MESSAGE_RUN:

            m_eventEmitter.emitEvent(
                EventType::EVENT_RUN_RCV);

            break;

        case ProtocolMessage::MESSAGE_IDLE:

            m_eventEmitter.emitEvent(
                EventType::EVENT_IDLE_RCV);

            break;

        case ProtocolMessage::MESSAGE_REBOOT:

            m_eventEmitter.emitEvent(
                EventType::EVENT_REBOOT);

            break;

        case ProtocolMessage::MESSAGE_GET_STATE:

            m_eventEmitter.emitEvent(
                EventType::EVENT_GET_STATE);

            break;  

        case ProtocolMessage::MESSAGE_DISABLED:

            m_eventEmitter.emitEvent(
                EventType::EVENT_DIS_RCV);

            break;

        case ProtocolMessage::MESSAGE_CMD:

            processCommand(m_buffer + 4);

            break;

        case ProtocolMessage::MESSAGE_UNKNOWN:
        default:

            // Mensagem desconhecida.
            // Futuramente poderemos gerar
            // EVENT_PROTOCOL_ERROR, se necessário.

            break;
        }
    }
    void ProtocolHandler::processCommand(const char *text )
    {
        const char * separator = strchr(text, ':');

        if (separator == nullptr){
            m_eventEmitter.emitEvent(
                EventType::EVENT_CMD_ERROR);
            return;     
        }
            
        Command command = {};
        
        size_t pathLength = static_cast<size_t>(separator - text);
       
        if (pathLength >= Command::PATH_SIZE)
        {
            m_eventEmitter.emitEvent(
                EventType::EVENT_CMD_ERROR);
            return;
        }

        strncpy(command.path, text, pathLength);
        command.path[pathLength] = '\0';

        const char *value = separator + 1;
        size_t valueLength = strlen(value);
        if (valueLength >= Command::VALUE_SIZE)
        {
            m_eventEmitter.emitEvent(
                EventType::EVENT_CMD_ERROR);
            return;
        }

        strncpy(command.value, value, valueLength);
        command.value[valueLength] = '\0';

        Event event = {};
        event.type = EventType::EVENT_CMD_RCV;
        event.command = command;
        m_eventEmitter.emitEvent(event);
    }

    void ProtocolHandler::send(ITransport &transport, ProtocolMessage message)
    {
        const char *text =
            protocolMessageToString(message);

        transport.write(
            reinterpret_cast<const uint8_t *>(text),
            strlen(text));

        const uint8_t newline = '\n';

        transport.write(
            &newline,
            1);
    }

    void ProtocolHandler::send(ITransport &transport, const char *message)
    {
        transport.write(
            reinterpret_cast<const uint8_t *>(message),
            strlen(message));

        const uint8_t newline = '\n';

        transport.write(
            &newline,
            1);
    }

    void ProtocolHandler::sendHeartbeat(
        ITransport &transport)
    {
        send(
            transport,
            ProtocolMessage::MESSAGE_HB);
    }

    bool ProtocolHandler::hasEvent() const
    {
        return m_eventEmitter.hasEvent();
    }

    Event ProtocolHandler::getEvent()
    {
        return m_eventEmitter.getEvent();
    }

void ProtocolHandler::sendState(ITransport& transport, State state)
{
    switch (state)
    {
        case State::STATE_UP:

            send(
                transport,
                ProtocolMessage::MESSAGE_UP
            );

            break;


        case State::STATE_CFG:

            send(
                transport,
                ProtocolMessage::MESSAGE_CFG
            );

            break;


        case State::STATE_RUN:

            send(
                transport,
                ProtocolMessage::MESSAGE_RUN
            );

            break;


        case State::STATE_IDLE:

            send(
                transport,
                ProtocolMessage::MESSAGE_IDLE
            );

            break;
            
        case State::STATE_DISABLED:

            send(
                transport,
                ProtocolMessage::MESSAGE_DISABLED
            );

            break;
    }
}

void ProtocolHandler::sendCommand(ITransport &transport, Command command)
{
    char message[256] = {};

    strncpy(message, command.path, Command::PATH_SIZE);
    strncat(message, "=", 1);
    strncat(message, command.value, Command::VALUE_SIZE);

    send(transport, message);   
}
}