/*
 * TimedTask.h
 * 
 * Created by Danilo Queiroz Barbosa, June 18, 2017.
 * Member of electronicdrops.com
 * 
 * 
 * Header of the class TimedTask 
 * A Simple way to use concurrent tasks.
 * 
 */


#ifndef TIMEDTASK_H
#define TIMEDTASK_H

#include "Arduino.h"




class TimedTask {


  private:
    uint32_t _iterations;
    uint8_t _active;
    uint32_t _interval;
    uint32_t _previous_run;
    void (*_function)();    
    void check_iterations();

  public:
    TimedTask(void (*func)(), uint32_t interval_);
    void run();
    void disable_iterations();
    uint8_t active() const;
    uint8_t active(uint8_t act);
    uint32_t interval() const;
    uint32_t interval(uint32_t interval_);
    uint32_t iterations() const;
    uint32_t iterations(uint32_t iter);
    void start();
    void start(uint32_t start_time);

};


#endif
