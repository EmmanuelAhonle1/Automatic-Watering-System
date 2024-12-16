// TimerInterrupt.cpp
#include "TimerInterrupt.hpp"

TimerInterrupt *TimerInterrupt::instance = nullptr;

TimerInterrupt::TimerInterrupt() : interruptCount(0), userCallback(nullptr)
{
    instance = this;
#ifdef ESP32
    timerMux = portMUX_INITIALIZER_UNLOCKED;
    timer = nullptr;
#endif
}

void TimerInterrupt::begin(uint32_t microseconds)
{
#ifdef ESP32
    // ESP32 Timer Setup
    timer = timerBegin(0, 80, true);                // Timer 0, prescaler 80, count-up
    timerAttachInterrupt(timer, &onTimerISR, true); // Edge triggered
    timerAlarmWrite(timer, microseconds, true);     // Set interval, auto-reload
    timerAlarmEnable(timer);                        // Start timer
#else
    // ESP8266 Timer Setup
    timer1_attachInterrupt(onTimerISR);
    timer1_enable(TIM_DIV256, TIM_EDGE, TIM_LOOP);

    // Calculate ticks for desired interval
    // 80MHz/256 = 312.5KHz
    // ticks = (312500/1000000) * microseconds
    uint32_t ticks = (microseconds * 312.5) / 1000;
    timer1_write(ticks);
#endif
}

void TimerInterrupt::stop()
{
#ifdef ESP32
    if (timer)
    {
        timerAlarmDisable(timer);
    }
#else
    timer1_disable();
#endif
}

void TimerInterrupt::resume()
{
#ifdef ESP32
    if (timer)
    {
        timerAlarmEnable(timer);
    }
#else
    timer1_enable(TIM_DIV256, TIM_EDGE, TIM_LOOP);
#endif
}

void TimerInterrupt::setCallback(void (*callback)())
{
    userCallback = callback;
}

uint32_t TimerInterrupt::getCount() const
{
    return interruptCount;
}

void TimerInterrupt::resetCount()
{
    interruptCount = 0;
}

#ifdef ESP32
void IRAM_ATTR TimerInterrupt::onTimerISR()
{
    if (instance)
    {
        portENTER_CRITICAL_ISR(&instance->timerMux);
        instance->interruptCount++;
        if (instance->userCallback)
        {
            instance->userCallback();
        }
        portEXIT_CRITICAL_ISR(&instance->timerMux);
    }
}
#else
void ICACHE_RAM_ATTR TimerInterrupt::onTimerISR()
{
    if (instance)
    {
        instance->interruptCount++;
        if (instance->userCallback)
        {
            instance->userCallback();
        }
    }
}
#endif