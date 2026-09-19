#pragma once
#include "NodeControl/StateMachine.h"
#include "NodeControl/Events.h"
#include "NodeControl/HeartBeatMonitor.h"
#include "NodeControl/ConfigMonitor.h"
#include "NodeControl/ProtocolHandler.h"
#include "NodeControl/JSONStatusWriter.h"
#include "NodeControl/ITransport.h"
#include "NodeControl/NodeController.h"
#include "NodeControl/StatusMonitor.h"
#include "NodeControl/StartupMode.h"

namespace node
{
    class Orquestrator
    {
    public:
        Orquestrator(ITransport &transport, const char* deviceConfig, StartupMode startupMode = StartupMode::MANAGED) : m_transportSystem(transport), m_deviceConfig(deviceConfig), m_startupMode(startupMode) {};
        void begin();
        void update();
        void handleEvent(const Event &event);
        void handleCommand(const Command &command);
        State getState() const { return m_stateMachine.getState(); }
        void addNode(INode& node);

    private:
        node::StateMachine m_stateMachine;
        node::HeartBeatMonitor m_heartBeatMonitor;
        node::ConfigMonitor m_configMonitor;
        node::ProtocolHandler m_protocol;
        node::ITransport &m_transportSystem;
        node::NodeController m_nodeController;
        node::StatusMonitor m_statusMonitor;

    private:
        const char* m_deviceConfig;
        StartupMode m_startupMode = StartupMode::MANAGED;

        void sendNodeStatusUpdate(ITransport &transport);
    };

}