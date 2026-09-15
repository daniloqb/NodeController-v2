#include "Orquestrator.h"

namespace node
{

    void Orquestrator::begin()
    {
        m_transportSystem.begin();
        m_protocol.begin();
        m_stateMachine.begin();
        m_heartBeatMonitor.begin();
    }

    void Orquestrator::update()
    {

        m_transportSystem.update();
        m_protocol.update(m_transportSystem);

        while (m_protocol.hasEvent())
        {
            Event event = m_protocol.getEvent();
            handleEvent(event);
        }

        m_stateMachine.update();

        m_heartBeatMonitor.update();
        while (m_heartBeatMonitor.hasEvent())
        {
            Event event = m_heartBeatMonitor.getEvent();
            handleEvent(event);
        }
    }

    void Orquestrator::handleEvent(const Event &event)
    {

        switch (event.type)
        {
        case EventType::EVENT_UP_ACK:
        case EventType::EVENT_CFG_ACK:
        case EventType::EVENT_RUN_RCV:
        case EventType::EVENT_IDLE_RCV:
        case EventType::EVENT_REBOOT:

            m_stateMachine.handleEvent(event);
            break;

        case EventType::EVENT_HB_ACK:
            m_heartBeatMonitor.handleEvent(event);
            break;

        case EventType::EVENT_HB_REQUEST:
            m_protocol.sendHeartbeat(m_transportSystem);
            break;

        case EventType::EVENT_HB_TIMEOUT:
            m_stateMachine.handleEvent(event);

            break;
        default:
            break;
        }
    }

}