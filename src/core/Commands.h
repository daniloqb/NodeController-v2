#pragma once
#include <stddef.h>
#include <stdint.h>
namespace node
{
 static constexpr size_t COMMAND_PATH_SIZE = 48;
 static constexpr size_t COMMAND_PAYLOAD_SIZE = 96;


    struct Command
    {
       
        char path[COMMAND_PATH_SIZE] = {};
        char payload[COMMAND_PAYLOAD_SIZE] = {};
        bool hasPayload = false;
    };


    enum class CommandResultType : uint8_t
    {
        ACK,
        RESPONSE,
        ERROR,
    };

    enum class CommandError : uint8_t
    {
        NONE,
        INVALID_STATE,
        INVALID_PAYLOAD,
        INVALID_FORMAT,
        NODE_NOT_FOUND,
        PROPERTY_NOT_FOUND,
        PROPERTY_NOT_SETTABLE,
        INTERNAL_ERROR,
    };  

    struct CommandResult
    {
        CommandResultType type = CommandResultType::ACK;
        CommandError error = CommandError::NONE;
        char payload[COMMAND_PAYLOAD_SIZE] = {};
        bool hasPayload = false;
    };

    const char* CommandErrorToString(CommandError error);
}