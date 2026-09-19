#pragma once
#include "NodeControl/ITransport.h"
#include "NodeControl/Events.h"
#include "NodeControl/NodeEvent.h"
#include "NodeControl/StateMachine.h"
#include "NodeControl/Commands.h"

#include "NodeControl/ProtocolMessage.h"

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

    void writeText(ITransport &transport, const char *text);
    void writeValue(ITransport &transport, const Value &value);
    void writeChar(ITransport &transport, char value);
    void writeCommandPath(ITransport &transport, const Command &command);

    void receive(uint8_t byte);
    void receiveMessage(const char *message);

    bool hasEvent() const;
    Event getEvent();

    bool hasCommand() const;
    Command getCommand();
    void releaseCommand();

    void sendHeartbeat(ITransport &transport);
    void sendState(ITransport &transport, State state);
    void sendNodeEvent(ITransport &transport, const NodeEvent &event);
    void sendCommand(ITransport &transport, const Command& command);
    void sendCommandResult(ITransport &transport, const Command &command, const CommandResult &result);
    void sendProgmem(ITransport &transport, const char *progmemString);
    void sendConfig(ITransport &transport);

  private:
    void processMessage();
    bool parseCommand();
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