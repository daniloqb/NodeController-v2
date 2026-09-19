#include "Orquestrator.h"

namespace node
{

    void Orquestrator::begin()
    {
        m_transportSystem.begin();
        m_protocol.begin();
        m_stateMachine.begin();
        m_heartBeatMonitor.begin();
        m_configMonitor.begin();
        m_statusMonitor.begin();
        m_nodeController.begin();

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

        while (m_protocol.hasCommand())
        {
            Command command = m_protocol.getCommand();

            handleCommand(command);
            m_protocol.releaseCommand();
        }

        m_stateMachine.update();

        m_heartBeatMonitor.update();
        while (m_heartBeatMonitor.hasEvent())
        {
            Event event = m_heartBeatMonitor.getEvent();
            handleEvent(event);
        }

        m_configMonitor.update();
        while (m_configMonitor.hasEvent())
        {
            Event event = m_configMonitor.getEvent();
            handleEvent(event);
        }

        m_nodeController.update();
        while (m_nodeController.hasEvent())
        {
            Event event = m_nodeController.getEvent();
            handleEvent(event);
        }

        NodeEvent nodeEvent = {};
        if (m_nodeController.pollEvent(nodeEvent))
        {
            m_protocol.sendNodeEvent(m_transportSystem, nodeEvent);
        }

        m_statusMonitor.update();
        while (m_statusMonitor.hasEvent())
        {
            Event event = m_statusMonitor.getEvent();
            handleEvent(event);
        }
    }

    void Orquestrator::handleEvent(const Event &event)
    {

        switch (event.type)
        {
        case EventType::EVENT_UP_ACK:
        case EventType::EVENT_CFG_ACK:
        case EventType::EVENT_REBOOT:

            m_stateMachine.handleEvent(event);
            m_configMonitor.handleEvent(event);
            m_heartBeatMonitor.handleEvent(event);
            break;
        case EventType::EVENT_RUN_RCV:
        case EventType::EVENT_IDLE_RCV:
        case EventType::EVENT_DIS_RCV:
            m_stateMachine.handleEvent(event);
            break;
        case EventType::EVENT_UP_TIMEOUT:
            m_protocol.sendState(m_transportSystem, m_stateMachine.getState());
            m_stateMachine.handleEvent(event);
            break;
        case EventType::EVENT_CFG_TIMEOUT:
            m_configMonitor.restartHandshake();
            m_stateMachine.handleEvent(event);
            break;

        case EventType::EVENT_HB_ACK:
            m_heartBeatMonitor.handleEvent(event);
            break;

        case EventType::EVENT_UP_REQUEST:
        case EventType::EVENT_GET_STATE:
            m_protocol.sendState(m_transportSystem, m_stateMachine.getState());
            break;
        case EventType::EVENT_CFG_REQUEST:
            m_protocol.sendConfig(m_transportSystem);
            break;

        case EventType::EVENT_HB_REQUEST:
            m_protocol.sendHeartbeat(m_transportSystem);
            break;

        case EventType::EVENT_HB_TIMEOUT:
            m_stateMachine.handleEvent(event);
            m_configMonitor.handleEvent(event);

            break;

        case EventType::EVENT_CMD_RCV:
            m_nodeController.handleEvent(event);
            break;

        case EventType::EVENT_STATUS_UPDATE:
            // Handle status update event
            if(m_stateMachine.getState() == State::STATE_RUN)
            {
                // Handle status update when in RUN state
             sendNodeStatusUpdate(m_transportSystem);
            }
            break;  

        case EventType::EVENT_CMD_ERROR:
            // m_nodeController.handleEvent(event);

            break;
        default:
            break;
        }
    }

    void Orquestrator::handleCommand(const Command &command)
    {
        State currentState = m_stateMachine.getState();

        if (currentState != State::STATE_IDLE && currentState != State::STATE_RUN)
        {
            CommandResult result{};

            result.type = CommandResultType::ERROR;
            result.error = CommandError::INVALID_STATE;

            m_protocol.sendCommandResult(m_transportSystem, command, result);

            return;
        }

        CommandResult result = m_nodeController.handleCommand(command);
        m_protocol.sendCommandResult(m_transportSystem, command, result);
    }
    void Orquestrator::sendNodeStatusUpdate(ITransport& transport   )
    {
        JSONStatusWriter writer(transport);
        m_nodeController.writeStatus(writer);
    }
}