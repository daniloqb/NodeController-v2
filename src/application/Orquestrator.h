#pragma once
#include "core/StateMachine.h"
#include "core/Events.h"
#include "agents/HeartBeatMonitor.h"
#include "protocol/ProtocolHandler.h"
#include "communication/ITransport.h"


namespace node
{
    class Orquestrator
    {
    public:
        Orquestrator(ITransport& transport): m_transportSystem(transport) {};
        void begin();
        void update();
        void handleEvent(const Event& event);  
        State getState() const { return m_stateMachine.getState(); }

    private:
        node::StateMachine m_stateMachine;
        node::HeartBeatMonitor m_heartBeatMonitor;
        node::ProtocolHandler m_protocol;
        node::ITransport& m_transportSystem;
    };

}