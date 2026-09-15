#include <Arduino.h>
#include "application/Orquestrator.h"
#include "core/Events.h"

node::SerialTransport serialTransport(Serial);
node::Orquestrator orquestrator(serialTransport);


void setup() {

  orquestrator.begin();
}

void loop() {
orquestrator.update();

}



