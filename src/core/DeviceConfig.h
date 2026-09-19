#pragma once

#include <Arduino.h>
#include <avr/pgmspace.h>

namespace node
{
    const char DEVICE_CONFIG[] PROGMEM = R"json(
{"nodes":{
    "l":{
        "name":"Led",
        "properties":{
            "s":{
                "name":"Status",
                "datatype":"boolean",
                "settable":true
            }
        }
    }
}}
)json";
}