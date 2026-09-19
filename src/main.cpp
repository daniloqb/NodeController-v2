#include <Arduino.h>
#include <NodeControl/NodeControl.h>
#include <NodeControl/LedNode.h>

namespace node
{
    const char DEVICE_CONFIG[] PROGMEM = R"json(
{"nodes":{
    "l":{
        "name":"LedNode",
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

node::SerialTransport serialTransport(Serial);
node::NodeDevice device(serialTransport, node::DEVICE_CONFIG, node::StartupMode::STANDALONE);
node::LedNode *ledNode = new node::LedNode(13);




void setup() {


  device.begin();
  device.addNode(*ledNode);

  Serial.println("Setup complete");
}

void loop() {
device.update();


}



