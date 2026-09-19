#include <NodeControl/Commands.h>

namespace node
{

const char *commandErrorToString(CommandError error)
{
    switch (error)
    {
    case CommandError::NONE:
        return "NONE";
    case CommandError::INVALID_STATE:
        return "INVALID_STATE";
    case CommandError::INVALID_PAYLOAD:
        return "INVALID_PAYLOAD";
    case CommandError::INVALID_FORMAT:
        return "INVALID_FORMAT";
    case CommandError::NODE_NOT_FOUND:
        return "NODE_NOT_FOUND";
    case CommandError::PROPERTY_NOT_FOUND:
        return "PROPERTY_NOT_FOUND";
    case CommandError::PROPERTY_NOT_SETTABLE:
        return "PROPERTY_NOT_SETTABLE";
    case CommandError::INTERNAL_ERROR:
        return "INTERNAL_ERROR";
    default:
        return "UNKNOWN_ERROR";
    }
}

}