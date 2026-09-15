/*
 * TimedTask.cpp
 * 
 * Created by Danilo Queiroz Barbosa, June 18, 2017.
 * Member of electronicdrops.com
 * 
 * 
 * Implementation of the class TimedTask 
 * A Simple way to use concurrent tasks.
 * 
 */




#include "Arduino.h"
#include "TimedTask.h"



TimedTask::TimedTask(void (*func)(), uint32_t interval_) {

  _interval = interval_;
  _function = func;
  _previous_run = 0;
  _active = 1;
  _iterations = -1;
}



void TimedTask::run() {
  uint32_t current_run = millis();

  if (_active) {
    if ((int32_t)(current_run - _previous_run) >= (int32_t)(_interval)) {
      _previous_run += _interval;
      if (_iterations != 0 ) {
        _function();
        check_iterations();
      } 
      else {
        _active = false;
      } 

    } 
  }
}

uint8_t TimedTask::active() const {
  return _active;
}

uint8_t TimedTask::active(uint8_t act) {
  _previous_run = millis();
  _active = act;
   return _active;
}

uint32_t TimedTask::interval() const {
  return _interval;
}

uint32_t TimedTask::interval(uint32_t interval_) {
  _interval = interval_;
  return _interval;
}

uint32_t TimedTask::iterations() const {
  return _iterations;
}

uint32_t TimedTask::iterations(uint32_t iter) {

  _iterations = iter;
  return _iterations;
}

void TimedTask::check_iterations() {

    if (_iterations > 0) {
      _iterations--;

    }
    if (_iterations == 0) {
      active(false);
      }  
}

void TimedTask::disable_iterations() {
  _iterations = -1;
}

void TimedTask::start() {
  _previous_run = millis();
}

void TimedTask::start(uint32_t start_time) {
  _previous_run = millis() + start_time -_interval;
} 
