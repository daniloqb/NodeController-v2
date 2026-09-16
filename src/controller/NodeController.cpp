#include "NodeController.h"
#include <Arduino.h>

namespace node
{

    void node::NodeController::begin()
    {
        m_ledNode.begin();
    }

    void node::NodeController::update()
    {
        m_ledNode.update();
    }

    void node::NodeController::handleEvent(const Event &event)
    {
        switch (event.type)
        {
        case EventType::EVENT_CMD_RCV:
            handleCommand(event.command);
            break;
        case EventType::EVENT_REBOOT:
            // Handle reboot event here
            break;

        default:
            break;
        }
    }

    bool node::NodeController::hasEvent() const
    {
        return m_eventEmitter.hasEvent();
    }

    Event node::NodeController::getEvent()
    {
        return m_eventEmitter.getEvent();
    }

    void NodeController::handleCommand(
        const Command &command)
    {
        if (strcmp(command.path, "/led/status") == 0)
        {
            if (strcmp(command.value, "true") == 0)
            {
                m_ledNode.setStatus(true);

                // gerar ACK
                return;
            }

            if (strcmp(command.value, "false") == 0)
            {
                m_ledNode.setStatus(false);

                // gerar ACK
                return;
            }

            // valor inválido
            // gerar ERROR
            return;
        }

        // path desconhecido
        // gerar ERROR
    }

}