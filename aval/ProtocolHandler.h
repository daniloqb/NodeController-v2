#pragma once
#include "protocol/ITransport.h"
#include "core/Events.h"

namespace node {

   class ProtocolHandler {
   public:
      ProtocolHandler();
      void begin();
      void update(ITransport& transport);
      bool hasEvent() const;
      Event getEvent();
      void sendHeartbeat(ITransport& transport);

    private:
      void processByte(uint8_t byte);
      void processMessage();
      void clearBuffer();

    private:
      static constexpr size_t BUFFER_SIZE = 256;
      char m_buffer[BUFFER_SIZE];
      size_t m_index = 0;
      Event m_event;
      bool m_hasEvent;
   };

}