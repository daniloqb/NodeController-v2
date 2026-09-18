#include "protocol/ProtocolHandler.h"
#include <string.h>
#include <stdlib.h>
#include <Arduino.h>

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

            if (!parseCommand(m_buffer))
            {
                m_eventEmitter.emitEvent(
                    EventType::EVENT_CMD_PARSE_ERROR);
            }
            break;

        case ProtocolMessage::MESSAGE_UNKNOWN:
        default:

            // Mensagem desconhecida.
            // Futuramente poderemos gerar
            // EVENT_PROTOCOL_ERROR, se necessário.

            break;
        }
    }

    bool ProtocolHandler::parseCommand(
        const char *text)
    {
        // Esperado:
        //
        // CMD:/led/status
        //
        // ou:
        //
        // CMD:/led/status:true

        const char *body = text + 4;

        if (*body != '/')
            return false;

        Command command{};

        const char *separator =
            strchr(body, ':');

        // ------------------------
        // GET
        // ------------------------

        if (separator == nullptr)
        {
            size_t pathLength =
                strlen(body);

            if (pathLength >=
                COMMAND_PATH_SIZE)
            {
                return false;
            }

            strcpy(command.path, body);

            command.hasPayload = false;
        }

        // ------------------------
        // SET
        // ------------------------

        else
        {
            size_t pathLength =
                separator - body;

            if (pathLength == 0 ||
                pathLength >= COMMAND_PATH_SIZE)
            {
                return false;
            }

            strncpy(
                command.path,
                body,
                pathLength);

            command.path[pathLength] = '\0';

            const char *payload =
                separator + 1;

            if (strlen(payload) >=
                COMMAND_PAYLOAD_SIZE)
            {
                return false;
            }

            strcpy(
                command.payload,
                payload);

            command.hasPayload = true;
        }

        m_command = command;
        m_hasCommand = true;



        return true;


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

    void ProtocolHandler::writeText(
        ITransport &transport,
        const char *text)
    {
        transport.write(
            reinterpret_cast<const uint8_t *>(text),
            strlen(text));
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

    bool ProtocolHandler::hasCommand() const
    {
        return m_hasCommand;
    }
    Command ProtocolHandler::getCommand()
    {
        Command command = m_command;
        m_command = {};
        m_hasCommand = false;

        return command;
    }

    void ProtocolHandler::sendState(ITransport &transport, State state)
    {
        switch (state)
        {
        case State::STATE_UP:

            send(
                transport,
                ProtocolMessage::MESSAGE_UP);

            break;

        case State::STATE_CFG:

            send(
                transport,
                ProtocolMessage::MESSAGE_CFG);

            break;

        case State::STATE_RUN:

            send(
                transport,
                ProtocolMessage::MESSAGE_RUN);

            break;

        case State::STATE_IDLE:

            send(
                transport,
                ProtocolMessage::MESSAGE_IDLE);

            break;

        case State::STATE_DISABLED:

            send(
                transport,
                ProtocolMessage::MESSAGE_DISABLED);

            break;
        }
    }

    void ProtocolHandler::sendCommand(ITransport &transport, Command command)
    {
        char message[256] = {};

        strncpy(message, command.path, COMMAND_PATH_SIZE);
        strncat(message, "=", 1);
        strncat(message, command.payload, COMMAND_PAYLOAD_SIZE);

        send(transport, message);
    }
 void ProtocolHandler::sendCommandResult( ITransport& transport, const Command& command, const CommandResult& result)
{
    switch (result.type)
    {
        case CommandResultType::ACK:
        {
            writeText(transport, "CMD_ACK:");
            writeText(transport, command.path);
            break;
        }


        case CommandResultType::RESPONSE:
        {
            writeText(
                transport,
                "CMD_RESPONSE:"
            );

            writeText(
                transport,
                command.path
            );

            writeText(transport, ":");

            writeText(
                transport,
                result.payload
            );

            break;
        }


        case CommandResultType::ERROR:
        {
            writeText(
                transport,
                "CMD_ERROR:"
            );

            writeText(
                transport,
                commandErrorToString(
                    result.error
                )
            );

            writeText(transport, ":");

            writeText(
                transport,
                command.path
            );

            break;
        }
    }


    const uint8_t newline = '\n';

    transport.write(
        &newline,
        1
    );
}

void ProtocolHandler::sendNodeEvent( ITransport& transport, const NodeEvent& event)
{
    switch (event.type)
    {
        case NodeEventType::DATA:
            writeText(
                transport,
                "NODE_DATA:"
            );
            break;


        case NodeEventType::TRIGGER:
            writeText(
                transport,
                "NODE_TRIGGER:"
            );
            break;


        case NodeEventType::ERROR:
            writeText(
                transport,
                "NODE_ERROR:"
            );

            writeText(
                transport,
                nodeErrorToString( event.error )
            );

            writeText(transport, ":");
            break;
    }


    writeText(
        transport,
        event.path
    );


    if (event.hasPayload)
    {
        writeText(transport, ":");

        writeText(
            transport,
            event.payload
        );
    }


    const uint8_t newline = '\n';

    transport.write(
        &newline,
        1
    );
}
}

