#pragma once
#include <stdint.h>

namespace node
{
    enum class EventType
    {
        EVENT_NONE,
        EVENT_INIT_COMPLETE,
        EVENT_ACK_UP,
        EVENT_ACK_CFG,
        EVENT_ACK_CMD,
        EVENT_CMD_RCV,
        EVENT_RUN_RCV,
        EVENT_IDLE_RCV,
        EVENT_CFG_TIMEOUT,
        EVENT_CMD_TIMEOUT,
        EVENT_HB_ACK,
        EVENT_HB_REQUEST,
        EVENT_HB_TIMEOUT,
        EVENT_REBOOT
    };

struct Event
{
    EventType type = EventType::EVENT_NONE;
    uint8_t nodeId = 0;
    int data = 0;

   
};

}