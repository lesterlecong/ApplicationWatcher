#ifndef TIMER_H
#define TIMER_H

typedef void (*TimerCallback)(void);

/**
 * @brief This class is used for the Periodic event.
 *
 */
class Timer
{
public:
    Timer();
    /**
     * @brief It will call the function after x milliseconds.
     *
     * @param timerCallback - holds the address of the function that will be called during the expiration of the time.
     * @param delay_ms
     */
    void setTimeout(TimerCallback timerCallback, int delay_ms);

    /**
     * @brief It will call the function every x milliseconds.
     *
     * @param TimerCallback - holds the address of the function that will be called everytime the time expired.
     * @param interval_ms
     */
    void setInterval(TimerCallback TimerCallback, int interval_ms);

    /**
     * @brief Stops the thread where the calling of function happened.
     *
     */
    void stop();

private:
    bool m_isClear;
};
#endif