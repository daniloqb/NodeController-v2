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
        MESSAGE_DISABLED,

        // ACKs
        MESSAGE_UP_ACK,
        MESSAGE_CFG_ACK,
        MESSAGE_HB_ACK,


        // Heartbeat
        MESSAGE_HB,

        // Comandos
        MESSAGE_CMD,
        MESSAGE_CMD_ACK,
        MESSAGE_CMD_RESPONSE,
        MESSAGE_CMD_ERROR,

        MESSAGE_NODE_DATA,
        MESSAGE_NODE_TRIGGER,
        MESSAGE_NODE_ERROR,
        
        MESSAGE_REBOOT,
        MESSAGE_GET_STATE,
    };

    const char* protocolMessageToString(ProtocolMessage message);

    ProtocolMessage stringToProtocolMessage(const char* text);
}