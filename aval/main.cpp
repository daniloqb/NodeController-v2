#include <Arduino.h>
#include "application/Orquestrator.h"
#include "communication/SerialTransport.h"
#include "core/StateMachine.h"

node::SerialTransport serialTransport(Serial);
node::Orquestrator orquestrator(serialTransport);


const char* toString(node::State s) {
  switch (s) {
    case node::State::STATE_UP: return "UP";
    case node::State::STATE_CFG: return "CFG";
    case node::State::STATE_IDLE: return "IDLE";
    case node::State::STATE_RUN: return "RUN";
    case node::State::STATE_DISABLED: return "DISABLED";
  }
  return "UNKNOWN";
}

void setup() {

  orquestrator.begin();
}

void loop() {
orquestrator.update();


}



