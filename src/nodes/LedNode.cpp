#include "nodes/LedNode.h"
#include "core/Events.h"
#include <Arduino.h>

namespace node
{
    node::LedNode::LedNode(uint8_t pin)
    {
        m_pin = pin;
    }

    void node::LedNode::SetStatus(const bool status)
    {
        m_status = status;
    }
    void node::LedNode::begin()
    {
        pinMode(m_pin, OUTPUT);
        SetStatus(false);
    }

    void node::LedNode::update()
    {
        digitalWrite(m_pin, m_status);
    }

    void node::LedNode::reset()
    {
        SetStatus(false);
    }

    bool node::LedNode::accepts(const Command &command) const
    {
        return strncmp(command.path, "/led/", strlen("/led/")) == 0;
    }

    bool node::LedNode::handleCommand(const Command &command)
    {

        if (strcmp(command.path, "/led/status") == 0)
        {
            if (command.hasPayload)
            {
                if (strcmp(command.payload, "true") == 0)
                {
                    SetStatus(true);
                    return true;
                }
                else if (strcmp(command.payload, "false") == 0)
                {
                    SetStatus(false);
                    return true;
                }
            }
        }
        return false;
    }

    const char *node::LedNode::getId() const
    {
        return "led";
    }

}