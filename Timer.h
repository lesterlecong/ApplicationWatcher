#ifndef TIMER_H
#define TIMER_H

typedef void (*TimerCallback)(void);

class Timer
{
public:
    Timer();
    void setTimeout(TimerCallback timerCallback, int delay_ms);
    void setInterval(TimerCallback TimerCallback, int interval_ms);
    void stop();

private:
    bool m_isClear;
};
#endif