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

    CommandResult node::LedNode::handleCommand(const Command &command)
    {

        CommandResult result = {};

        // Handle the "/led/status" command
        if (strcmp(command.path, "/led/status") != 0)
        {
            result.type = CommandResultType::ERROR;          // Assuming CommandResult has a type field and CommandResultType::ERROR exists
            result.error = CommandError::PROPERTY_NOT_FOUND; // Assuming CommandError::PROPERTY_NOT_FOUND exists
            return result;
        }

        if (strcmp(command.path, "/led/status") == 0)
        {
            if (command.hasPayload) //SET
            {
                if (strcmp(command.payload, "true") == 0)
                {
                    SetStatus(true);
                    result.type = CommandResultType::ACK;
                    return result;
                }
                else if (strcmp(command.payload, "false") == 0)
                {
                    SetStatus(false);
                    result.type = CommandResultType::ACK;
                    return result;
                }
            }
            if (!command.hasPayload) //GET
            {
                result.type = CommandResultType::RESPONSE;
                strcpy(result.payload, m_status ? "true" : "false");
                result.hasPayload = true;
                return result;
            }
        }

        result.type = CommandResultType::ERROR;
        result.error = CommandError::INVALID_PAYLOAD;

        return result;
    }

    bool node::LedNode::pollEvent(NodeEvent& event)
    {
        return false;
    }

    const char *node::LedNode::getId() const
    {
        return "led";
    }

}