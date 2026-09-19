#include "Button.h"

namespace node
{
    node::ButtonNode::ButtonNode(uint8_t pin, bool pullDown)
        : m_pin(pin), m_status(false), m_pullDown(pullDown)
    {
    }
    void node::ButtonNode::begin()
    {
        pinMode(m_pin, INPUT);
        m_eventPending = false;
    }
    void node::ButtonNode::update()
    {
        
        bool newStatus = digitalRead(m_pin);
        if (m_pullDown)
        {
            newStatus = !newStatus;
        }
        m_eventPending = (newStatus != m_status);
        
        m_status = newStatus;

    }

    void node::ButtonNode::reset()
    {
        m_status = false;
        m_eventPending = false;
    }

    char node::ButtonNode::getId() const
    {
        return 'b';
    }

    CommandResult node::ButtonNode::handleCommand(const Command &command)
    {

        CommandResult result{};

        if (command.propertyId != 's')
        {
            result.type =
                CommandResultType::ERROR;

            result.error =
                CommandError::PROPERTY_NOT_FOUND;

            return result;
        }

        if (!command.hasPayload)
        {
            result.type =
                CommandResultType::RESPONSE;

            result.value =
                makeBooleanValue(m_status);

            return result;
        }

        result.type =
            CommandResultType::ERROR;

        result.error =
            CommandError::INVALID_PAYLOAD;

        return result;
    }

    bool node::ButtonNode::pollEvent(NodeEvent &event)
    {
        if (m_eventPending)
        {
            event.type = node::NodeEventType::TRIGGER;
            event.error = node::NodeError::NONE;
            
            strcpy(event.path,"/b/s");
            strcpy(event.payload, m_status ? "true" : "false");
            event.hasPayload = true;

            m_eventPending = false;
            return true;
        }
        return false;
    }

    void node::ButtonNode::writeStatus(IStatusWriter &statusWriter) const
    {
        statusWriter.writeProperty('s', makeBooleanValue(m_status));
    }
} // namespace node