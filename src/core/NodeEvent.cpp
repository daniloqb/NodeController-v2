#include "core/NodeEvent.h"

namespace node
{
    const char* nodeErrorToString(NodeError error)
    {
        switch (error)
        {
            case NodeError::READ_FAILED:
                return "READ_FAILED";

            case NodeError::DEVICE_FAULT:
                return "DEVICE_FAULT";

            case NodeError::INVALID_DATA:
                return "INVALID_DATA";

            case NodeError::NONE:
            default:
                return "NONE";
        }
    }
}