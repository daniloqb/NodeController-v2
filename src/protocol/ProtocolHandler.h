#pragma once
#include "communication/ITransport.h"
#include "core/Events.h"
#include "core/NodeEvent.h"
#include "core/StateMachine.h"
#include "core/Commands.h"

#include "protocol/ProtocolMessage.h"

namespace node
{

  class ProtocolHandler
  {
  public:
    ProtocolHandler() = default;
    void begin();
    void update(ITransport &transport);
    void send(ITransport &transport, ProtocolMessage message);
    void send(ITransport &transport, const char *message);

    void writeText(ITransport& transport, const char* text);

    void receive(uint8_t byte);
    void receiveMessage(const char *message);

    bool hasEvent() const;
    Event getEvent();

    bool hasCommand() const;
    Command getCommand();

    void sendHeartbeat(ITransport &transport);
    void sendState(ITransport &transport, State state);
        void sendNodeEvent(ITransport &transport, const NodeEvent& event);
    void sendCommand(ITransport &transport, Command command);
    void sendCommandResult(ITransport &transport, const Command& command, const CommandResult& result);

  private:
    void processMessage();
    bool parseCommand(const char *text);
    void clearBuffer();

  private:
    static constexpr size_t BUFFER_SIZE = 64;
    char m_buffer[BUFFER_SIZE];
    size_t m_index = 0;
    EventEmitter m_eventEmitter;
    Command m_command{};
    bool m_hasCommand = false;
  };

}