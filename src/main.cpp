#include <Arduino.h>
#include "application/Orquestrator.h"
#include "communication/SerialTransport.h"
#include "core/StateMachine.h"

node::SerialTransport serialTransport(Serial);
node::Orquestrator orquestrator(serialTransport);


void setup() {

  orquestrator.begin();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("Setup complete");
}

void loop() {
orquestrator.update();


}



