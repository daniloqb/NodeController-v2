#pragma once
#include <stddef.h>
#include <stdint.h>


namespace node
{

    class ITransport
    {
    public:
        virtual ~ITransport() = default;

        virtual void write(const uint8_t *data, size_t length) = 0;
        virtual uint8_t read() = 0;
        virtual bool available() const = 0;

        virtual void begin() = 0;
        virtual void update() = 0;
    };


}