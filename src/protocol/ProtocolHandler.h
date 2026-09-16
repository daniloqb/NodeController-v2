#pragma once
#include "communication/ITransport.h"
#include "core/Events.h"
#include "core/StateMachine.h"
#include "protocol/ProtocolMessage.h"

namespace node
{

  class ProtocolHandler
  {
  public:
    ProtocolHandler() = default;
    void begin();
    void update(ITransport &transport);
    void receive(uint8_t byte);
    void receiveMessage(const char *message);

    bool hasEvent() const;
    Event getEvent();
    void sendHeartbeat(ITransport &transport);
    void sendState(ITransport &transport, State state);
    void sendCommand(ITransport &transport, Command command);
    void send(ITransport &transport, ProtocolMessage message);
  private : 
    void processMessage();
    void processCommand(const char *text);
    void clearBuffer();
   
    void send(ITransport &transport, const char *message);

  private:
    static constexpr size_t BUFFER_SIZE = 256;
    char m_buffer[BUFFER_SIZE];
    size_t m_index = 0;
    EventEmitter m_eventEmitter;
  };

}