#include "Orquestrator.h"

namespace node
{

    void Orquestrator::begin()
    {
        m_transport.begin();
        m_protocol.begin();
        m_stateMachine.begin();
        m_heartBeatManager.begin();
    }

    void Orquestrator::update()
    {

        m_transport.update();
        m_protocol.update(m_transport);

        while (m_protocol.hasEvent())
        {
            Event event = m_protocol.getEvent();
            handleEvent(event);
        }

        m_stateMachine.update();
        Event event;
        m_heartBeatManager.update();
        while (m_heartBeatManager.hasEvent())
        {
            Event event = m_heartBeatManager.getEvent();
            handleEvent(event);
        }
    }

    void Orquestrator::handleEvent(const Event &event)
    {

        switch (event.type)
        {
        case EventType::EVENT_ACK_UP:
        case EventType::EVENT_ACK_CFG:
        case EventType::EVENT_RUN_RCV:
        case EventType::EVENT_IDLE_RCV:
        case EventType::EVENT_REBOOT:

            m_stateMachine.handleEvent(event);
            break;

        case EventType::EVENT_HB_ACK:
            m_heartBeatManager.handleEvent(event);
            break;

        case EventType::EVENT_HB_REQUEST:
            m_protocol.sendHeartbeat(m_transport);
            break;

        case EventType::EVENT_HB_TIMEOUT:
            m_stateMachine.handleEvent(event);

            break;
        }
    }

}