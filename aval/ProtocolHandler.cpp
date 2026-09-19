#include <NodeControl/ProtocolHandler.h>
#include <string.h>
#include <stdlib.h>
#include <avr/pgmspace.h>


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

        if (m_hasCommand)
            return;

        while (transport.available())
        {
            uint8_t byte = transport.read();
            receive(byte);

            if (m_hasCommand)
                return;

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

                if (!m_hasCommand)
                {
                    clearBuffer();
                }
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

            // FUTURAMENTE ERRO: BUFFER OVERFLOW
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

        if (!m_hasCommand)
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

            if (!parseCommand())
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

    bool ProtocolHandler::parseCommand()
    {
        // Exemplos:
        //
        // CMD:/l/s
        // CMD:/l/s:1

        char *body = m_buffer + 4;

        // Esperamos pelo menos:
        //
        // /l/s
        //
        // 0123

        if (body[0] != '/' ||
            body[1] == '\0' ||
            body[2] != '/')
        {
            return false;
        }

        m_command = {};

        m_command.nodeId = body[1];
        m_command.propertyId = body[3];

        // Depois da propriedade só podem existir:
        //
        // '\0'
        //
        // ou
        //
        // ':' + payload

        if (body[3] == '\0' || body[4] == '\0')
        {
            // GET
            m_command.payload = nullptr;
            m_command.hasPayload = false;
        }
        else if (body[4] == ':')
        {
            // SET

            if (body[5] == '\0')
            {
                // Payload vazio.
                // Por enquanto consideramos inválido.
                return false;
            }

            m_command.payload = &body[5];
            m_command.hasPayload = true;
        }
        else
        {
            return false;
        }

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

    void ProtocolHandler::writeChar(ITransport &transport, char value)
    {
        uint8_t byte =
            static_cast<uint8_t>(value);

        transport.write(
            &byte,
            1);
    }

    void ProtocolHandler::writeValue(ITransport &transport, const Value &value)
    {
        char buffer[16];

        switch (value.type)
        {
        case ValueType::BOOLEAN:
        {
            writeText(transport, value.data.booleanValue ? "1" : "0");
            break;
        }
        case ValueType::INTEGER:
        {
            ltoa(value.data.integerValue, buffer, 10);
            writeText(transport, buffer);
            break;
        }
        case ValueType::FLOAT:
        {
            dtostrf(value.data.floatValue, 0, 6, buffer);
            writeText(transport, buffer);
            break;
        }

        case ValueType::NONE:
        default:
            break;
        }
    }

    void ProtocolHandler::writeCommandPath(ITransport &transport, const Command &command)
    {
        writeChar(transport, '/');

        writeChar(
            transport,
            command.nodeId);

        writeChar(transport, '/');

        if (command.propertyId != '\0')
            writeChar(transport, command.propertyId);
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
    void ProtocolHandler::releaseCommand()
    {
        m_command = {};
        m_hasCommand = false;
        clearBuffer();
    }

    Command ProtocolHandler::getCommand()
    {

        return m_command;
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

    void ProtocolHandler::sendCommand(ITransport &transport, const Command &command)
    {
        writeText(transport, "CMD:");

        writeChar(transport, '/');
        writeChar(transport, command.nodeId);

        writeChar(transport, '/');
        writeChar(transport, command.propertyId);

        if (command.hasPayload &&
            command.payload != nullptr)
        {
            writeChar(transport, ':');
            writeText(transport, command.payload);
        }

        writeChar(transport, '\n');
    }

    void ProtocolHandler::sendCommandResult(ITransport &transport, const Command &command, const CommandResult &result)
    {
        switch (result.type)
        {
        case CommandResultType::ACK:
        {
            writeText(
                transport,
                "CMD_ACK:");

            writeCommandPath(
                transport,
                command);

            break;
        }

        case CommandResultType::RESPONSE:
        {
            writeText(
                transport,
                "CMD_RESPONSE:");

            writeCommandPath(
                transport,
                command);

            writeChar(
                transport,
                ':');

            writeValue(
                transport,
                result.value);

            break;
        }

        case CommandResultType::ERROR:
        {
            writeText(
                transport,
                "CMD_ERROR:");

            writeText(
                transport,
                commandErrorToString(
                    result.error));

            writeChar(
                transport,
                ':');

            writeCommandPath(
                transport,
                command);

            break;
        }
        }

        writeChar(
            transport,
            '\n');
    }

    void ProtocolHandler::sendProgmem(ITransport &transport, const char *progmemString)
    {
        while (true)
        {
            char c = pgm_read_byte(progmemString++);
            if (c == '\0')
            {
                break;
            }
            uint8_t byte = static_cast<uint8_t>(c);
            transport.write(&byte, 1);
        }
    }

    void ProtocolHandler::sendConfig(ITransport &transport, const char* deviceConfig)
    {
        writeText(
            transport,
            "CFG:");

        sendProgmem(
            transport,
            deviceConfig);

        writeText(
            transport,
            "\n");
    }

    void ProtocolHandler::sendNodeEvent(ITransport &transport, const NodeEvent &event)
    {
        switch (event.type)
        {
        case NodeEventType::DATA:
            writeText(
                transport,
                "NODE_DATA:");
            break;

        case NodeEventType::TRIGGER:
            writeText(
                transport,
                "NODE_TRIGGER:");
            break;

        case NodeEventType::ERROR:
            writeText(
                transport,
                "NODE_ERROR:");

            writeText(
                transport,
                nodeErrorToString(event.error));

            writeText(transport, ":");
            break;
        }

        writeText(
            transport,
            event.path);

        if (event.hasPayload)
        {
            writeText(transport, ":");

            writeText(
                transport,
                event.payload);
        }

        const uint8_t newline = '\n';

        transport.write(
            &newline,
            1);
    }

}
