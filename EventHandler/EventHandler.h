#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <list>
#include <thread>
#include <atomic>

using namespace std;

typedef void (*EventCallback)(const wchar_t *, const wchar_t *);

class EventHandler
{
public:
    EventHandler();
    virtual ~EventHandler();

    void addApp(const wchar_t *sAppName);
    void callback(EventCallback callback);
    bool run();
    void stop();
    bool isRunning() const;

protected:
    virtual void runProcess() = 0;
    void abortAndJoin();
    list<const wchar_t *> m_appList;
    EventCallback m_callback;
    thread m_thread;
    atomic_bool m_isRunning;
    atomic_bool m_abortRequested;
};
#endif