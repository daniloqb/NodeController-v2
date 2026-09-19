#include <NodeControl/LedNode.h>
#include <NodeControl/Events.h>
#include <NodeControl/Value.h>
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


   CommandResult LedNode::handleCommand(
    const Command& command)
{
    CommandResult result{};

    // O LedNode atualmente só possui
    // a propriedade 's' = status

    if (command.propertyId != 's')
    {
        result.type =
            CommandResultType::ERROR;

        result.error =
            CommandError::PROPERTY_NOT_FOUND;

        return result;
    }


    // -------------------------
    // GET
    //
    // CMD:/l/s
    // -------------------------

    if (!command.hasPayload)
    {
        result.type =
            CommandResultType::RESPONSE;

        result.value =
            makeBooleanValue(m_status);

        return result;
    }


    // -------------------------
    // SET TRUE
    //
    // CMD:/l/s:1
    // -------------------------

    if (strcmp(command.payload, "1") == 0)
    {
        SetStatus(true);

        result.type =
            CommandResultType::ACK;

        return result;
    }


    // -------------------------
    // SET FALSE
    //
    // CMD:/l/s:0
    // -------------------------

    if (strcmp(command.payload, "0") == 0)
    {
        SetStatus(false);

        result.type =
            CommandResultType::ACK;

        return result;
    }


    // -------------------------
    // INVALID VALUE
    // -------------------------

    result.type =
        CommandResultType::ERROR;

    result.error =
        CommandError::INVALID_PAYLOAD;

    return result;
}

    bool node::LedNode::pollEvent(NodeEvent& event)
    {
        return false;
    }

    char node::LedNode::getId() const
    {
        return 'l';
    }

    void node::LedNode::writeStatus(IStatusWriter& writer) const
    {
        writer.writeProperty('s', makeBooleanValue(m_status)
        );
    }

}