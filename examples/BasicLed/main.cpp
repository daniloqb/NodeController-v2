#include <Arduino.h>
#include <NodeControl/NodeControl.h>
#include <NodeControl/LedNode.h>

node::SerialTransport serialTransport(Serial);
node::NodeDevice device(serialTransport);
node::LedNode *ledNode = new node::LedNode(13);

void setup() {

  device.begin();
  device.addNode(*ledNode);

  Serial.println("Setup complete");
}

void loop() {
device.update();


}



