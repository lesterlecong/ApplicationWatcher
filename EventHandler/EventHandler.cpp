#include "EventHandler.h"
#include <list>
#include <thread>
#include <atomic>
#include <stdio.h>

EventHandler::EventHandler()
{
}

EventHandler::~EventHandler()
{
    abortAndJoin();
}

void EventHandler::addApp(const wchar_t *sAppName)
{
    m_appList.push_back(sAppName);
}

void EventHandler::callback(EventCallback callback)
{
    m_callback = callback;
}

bool EventHandler::run()
{
    try
    {
        m_thread = thread(&EventHandler::runProcess, this);
    }
    catch (...)
    {
#ifdef ENABLE_DEBUG
        printf("Error in running the thread\r\n");
#endif
        return false;
    }

    return true;
}

void EventHandler::stop()
{
    abortAndJoin();
}

bool EventHandler::isRunning() const
{
    return m_isRunning.load();
}

void EventHandler::abortAndJoin()
{
#ifdef ENABLE_DEBUG
    printf("Aborting thread\r\n");
#endif
    m_abortRequested.store(true);
    if (m_thread.joinable())
    {
        m_thread.join();
    }
}
