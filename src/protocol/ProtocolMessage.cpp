#include <NodeControl/ProtocolMessage.h>
#include <string.h>

namespace node
{
    const char* protocolMessageToString(ProtocolMessage message)
    {
        switch (message)
        {
        case ProtocolMessage::MESSAGE_UP: return "UP";
        case ProtocolMessage::MESSAGE_CFG: return "CFG";
        case ProtocolMessage::MESSAGE_RUN: return "RUN";
        case ProtocolMessage::MESSAGE_IDLE: return "IDLE";
        case ProtocolMessage::MESSAGE_DISABLED: return "DIS";
        case ProtocolMessage::MESSAGE_UP_ACK: return "UP_ACK";
        case ProtocolMessage::MESSAGE_CFG_ACK: return "CFG_ACK";
        case ProtocolMessage::MESSAGE_HB_ACK: return "HB_ACK";
        case ProtocolMessage::MESSAGE_HB: return "HB";
        case ProtocolMessage::MESSAGE_CMD: return "CMD";
        case ProtocolMessage::MESSAGE_CMD_ACK: return "CMD_ACK";
        case ProtocolMessage::MESSAGE_CMD_RESPONSE: return "CMD_RESPONSE";
        case ProtocolMessage::MESSAGE_CMD_ERROR: return "CMD_ERROR";
        case ProtocolMessage::MESSAGE_NODE_DATA: return "NODE_DATA";
        case ProtocolMessage::MESSAGE_NODE_TRIGGER: return "NODE_TRIGGER";
        case ProtocolMessage::MESSAGE_NODE_ERROR: return "NODE_ERROR";
        case ProtocolMessage::MESSAGE_REBOOT: return "REBOOT";
        case ProtocolMessage::MESSAGE_GET_STATE: return "GET_STATE";
        case ProtocolMessage::MESSAGE_UNKNOWN:
        default: return "";
        }
    }


    ProtocolMessage stringToProtocolMessage(const char* text)
    {
        if (strcmp(text, "UP") == 0)
            return ProtocolMessage::MESSAGE_UP;

        if (strcmp(text, "CFG") == 0)
            return ProtocolMessage::MESSAGE_CFG;

        if (strcmp(text, "RUN") == 0)
            return ProtocolMessage::MESSAGE_RUN;

        if (strcmp(text, "IDLE") == 0)
            return ProtocolMessage::MESSAGE_IDLE;

        if (strcmp(text, "UP_ACK") == 0)
            return ProtocolMessage::MESSAGE_UP_ACK;

        if (strcmp(text, "CFG_ACK") == 0)
            return ProtocolMessage::MESSAGE_CFG_ACK;

        if (strcmp(text, "HB_ACK") == 0)
            return ProtocolMessage::MESSAGE_HB_ACK;

        if (strcmp(text, "CMD_ACK") == 0)
            return ProtocolMessage::MESSAGE_CMD_ACK;

        if (strcmp(text, "HB") == 0)
            return ProtocolMessage::MESSAGE_HB;

        if (strcmp(text, "CMD_ERROR") == 0)
            return ProtocolMessage::MESSAGE_CMD_ERROR;

        if (strcmp(text, "CMD") == 0)
            return ProtocolMessage::MESSAGE_CMD;

        if (strcmp(text, "REBOOT") == 0)
            return ProtocolMessage::MESSAGE_REBOOT;
            
        if (strcmp(text, "DIS") == 0)
            return ProtocolMessage::MESSAGE_DISABLED;

        if (strcmp(text, "GET_STATE") == 0)
            return ProtocolMessage::MESSAGE_GET_STATE;


        //
        // CMD é especial porque posteriormente poderá ser:
        //
        // CMD:3:MOVE:120
        //
        if (strncmp(text, "CMD:", 4) == 0)
            return ProtocolMessage::MESSAGE_CMD;

        return ProtocolMessage::MESSAGE_UNKNOWN;
    }
}
