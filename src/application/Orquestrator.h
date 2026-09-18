#pragma once
#include "core/StateMachine.h"
#include "core/Events.h"
#include "agents/HeartBeatMonitor.h"
#include "agents/ConfigMonitor.h"
#include "protocol/ProtocolHandler.h"
#include "communication/ITransport.h"
#include "controller/NodeController.h"


namespace node
{
    class Orquestrator
    {
    public:
        Orquestrator(ITransport& transport): m_transportSystem(transport) {};
        void begin();
        void update();
        void handleEvent(const Event& event);  
        void handleCommand(const Command& command);
        State getState() const { return m_stateMachine.getState(); }

    private:
        node::StateMachine m_stateMachine;
        node::HeartBeatMonitor m_heartBeatMonitor;
        node::ConfigMonitor m_configMonitor;
        node::ProtocolHandler m_protocol;
        node::ITransport& m_transportSystem;
        node::NodeController m_nodeController;
    };

}