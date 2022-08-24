#ifndef WINDOWSLOGGER_H
#define WINDOWSLOGGER_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#include <LoggerImpl.h>
#ifndef WINDOWS_INCLUDE
#define WINDOWS_INCLUDE
#include <windows.h>
#endif

class WindowsLogger : public LoggerImpl
{
public:
    WindowsLogger();
    ~WindowsLogger();

    virtual void write(LogParam loggerDetails);

private:
    WORD eventLogType(LogType logType);
    HANDLE m_hEventLog;
};

#endif
#endif