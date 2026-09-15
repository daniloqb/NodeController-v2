#pragma once
#include "core/StateMachine.h"
#include "core/Events.h"
#include "communication/HeartBeatManager.h"
#include "protocol/ProtocolHandler.h"
#include "protocol/SerialTransport.h"


namespace node
{
    class Orquestrator
    {
    public:
        Orquestrator(ITransport& transport): m_transport(transport) {};
        void begin();
        void update();
        void handleEvent(const Event& event);  

    private:
        node::StateMachine m_stateMachine;
        node::HeartBeatManager m_heartBeatManager;
        node::ProtocolHandler m_protocol;
        node::ITransport& m_transport;
    };

}