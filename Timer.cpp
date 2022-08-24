#include "Timer.h"
#include <thread>
#include <chrono>

using namespace std;

Timer::Timer()
{
    m_isClear = false;
}

void Timer::setTimeout(TimerCallback timerCallback, int delay_ms)
{
    m_isClear = false;
    thread t([=]()
             {
        if(m_isClear) return;
        this_thread::sleep_for(chrono::milliseconds(delay_ms));
        if(m_isClear) return;
        if(timerCallback) timerCallback(); });

    t.detach();
}

void Timer::setInterval(TimerCallback timerCallback, int interval_ms)
{
    m_isClear = false;
    thread t([=]()
             {
        while(true)
        {
            if(m_isClear) return;
            this_thread::sleep_for(chrono::milliseconds(interval_ms));
            if(m_isClear) return;
            if(timerCallback) timerCallback();
        } });

    t.detach();
}

void Timer::stop()
{
    m_isClear = true;
}