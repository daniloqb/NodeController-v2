#pragma once

namespace node
{
    enum class ProtocolMessage
    {
        MESSAGE_UNKNOWN,

        // Estados publicados pelo Node
        MESSAGE_UP,
        MESSAGE_CFG,
        MESSAGE_RUN,
        MESSAGE_IDLE,

        // ACKs
        MESSAGE_UP_ACK,
        MESSAGE_CFG_ACK,
        MESSAGE_HB_ACK,
        MESSAGE_CMD_ACK,

        // Heartbeat
        MESSAGE_HB,

        // Comandos
        MESSAGE_CMD,
        MESSAGE_REBOOT,
        MESSAGE_GET_STATE
    };

    const char* protocolMessageToString(ProtocolMessage message);

    ProtocolMessage stringToProtocolMessage(const char* text);
}