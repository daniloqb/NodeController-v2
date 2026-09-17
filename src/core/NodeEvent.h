#pragma once

#include <stdint.h>
#include <stddef.h>

namespace node
{

    static constexpr size_t NODE_EVENT_PATH_SIZE = 48;
    static constexpr size_t NODE_EVENT_PAYLOAD_SIZE = 96;

    enum class NodeEventType : uint8_t
    {
        DATA,
        TRIGGER,
        ERROR,
    };

    enum class NodeError : uint8_t
    {
        NONE,
        READ_FAILED,
        DEVICE_FAULT,
        INVALID_DATA,
    };

    struct NodeEvent
    {
        NodeEventType type = NodeEventType::DATA;
        NodeError error = NodeError::NONE;
        char path[NODE_EVENT_PATH_SIZE] = {};
        char payload[NODE_EVENT_PAYLOAD_SIZE] = {};
        bool hasPayload = false;
    };
}