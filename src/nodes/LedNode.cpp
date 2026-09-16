#include "LedNode.h"
#include <Arduino.h>

namespace node
{
    void node::LedNode::begin()
    {
        pinMode(LED_BUILTIN, OUTPUT);
        m_status = false;
    }

    void node::LedNode::update()
    {
        digitalWrite(LED_BUILTIN, m_status);
    }

    void node::LedNode::setStatus(bool status)
    {
        m_status = status;
        digitalWrite(LED_BUILTIN, m_status);
    }

    bool node::LedNode::getStatus() const
    {
        return m_status;
    }

}