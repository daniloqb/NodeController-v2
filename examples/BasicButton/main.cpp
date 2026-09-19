#include <Arduino.h>
#include <NodeControl/NodeControl.h>
#include "devices/Button.h"

namespace node
{
    const char DEVICE_CONFIG[] PROGMEM = R"json(
{"nodes":{
    "b":{
        "name":"Button",
        "properties":{
            "s":{
                "name":"Status",
                "datatype":"boolean",
                "settable":false
            }
        }
    }
}}
)json";
}

node::SerialTransport serialTransport(Serial);
node::NodeDevice device(serialTransport, node::DEVICE_CONFIG);
node::ButtonNode *buttonNode = new node::ButtonNode(2,true);




void setup() {


  device.begin();
  device.addNode(*buttonNode);

  Serial.println("Setup complete");
}

void loop() {
device.update();


}



