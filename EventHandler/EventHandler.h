#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <list>
#include <thread>
#include <atomic>

using namespace std;

typedef void (*EventCallback)(const wchar_t *, const wchar_t *);

/**
 * @brief This class handles the system events.
 *        It will run on the different thread so that
 *        it will not interfere other processes during the listening of the events.
 *
 */
class EventHandler
{
public:
    EventHandler();
    virtual ~EventHandler();

    /**
     * @brief Setting the callback function if an event was found.
     *
     * @param callback - holds the address of the function
     */
    void callback(EventCallback callback);

    /**
     * @brief This method will make a new thread to run the process of listening to the events.
     *
     * @return true - if successfully run the thread.
     * @return false - if unsuccessfully run the thread.
     */
    bool run();

    /**
     * @brief This method will make the thread stop.
     *
     */
    void stop();

    /**
     * @brief This method will check if the current thread is running or not.
     *
     * @return true
     * @return false
     */
    bool isRunning() const;

protected:
    virtual void runProcess() = 0;
    void abortAndJoin();
    EventCallback m_callback;
    thread m_thread;
    atomic_bool m_isRunning;
    atomic_bool m_abortRequested;
};
#endif