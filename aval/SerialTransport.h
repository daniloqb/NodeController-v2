#pragma once
#include "protocol/ITransport.h"
#include <HardwareSerial.h>

namespace node {
    class SerialTransport : public ITransport
    {

    public:
        SerialTransport(HardwareSerial &serial) : m_serial(serial) {};

        void begin() override
        {
            m_serial.begin(115200);
        }

        void update() override
        {
        }

        bool available() const override
        {
            return m_serial.available() > 0;
        }

        uint8_t read() override
        {
            return static_cast<uint8_t>(m_serial.read());
        }

        void write(const uint8_t *data, size_t length) override
        {
            m_serial.write(data, length);
        }

    private:
        HardwareSerial &m_serial;
    };

}