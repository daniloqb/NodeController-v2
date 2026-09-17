#pragma once
namespace node
{
    struct Command
    {
        static constexpr size_t PATH_SIZE = 32;
        static constexpr size_t PAYLOAD_SIZE = 32;

        char path[PATH_SIZE] = {};
        char payload[PAYLOAD_SIZE] = {};
        bool hasPayload = false;
    };

}