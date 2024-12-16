// TimerInterrupt.h
#ifndef TIMER_INTERRUPT_H
#define TIMER_INTERRUPT_H

#include <Arduino.h>

class TimerInterrupt
{
public:
    TimerInterrupt();
    void begin(uint32_t microseconds);
    void stop();
    void resume();
    void setCallback(void (*callback)());
    uint32_t getCount() const;
    void resetCount();

private:
    static TimerInterrupt *instance;
    volatile uint32_t interruptCount;
    void (*userCallback)();

#ifdef ESP32
    hw_timer_t *timer;
    static void IRAM_ATTR onTimerISR();
    portMUX_TYPE timerMux;
#else
    static void ICACHE_RAM_ATTR onTimerISR();
#endif

    friend void setupTimerISR();
};

#endif