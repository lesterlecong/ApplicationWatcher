#include "WindowsLogger.h"

#ifndef WINDOWS_INCLUDE
#define WINDOWS_INCLUDE
#include <windows.h>
#endif
#include <wchar.h>
#include <MeldCX_provider.h>

#define PROVIDER_NAME "MeldCX"
WindowsLogger::WindowsLogger()
{
    m_hEventLog = RegisterEventSource(NULL, PROVIDER_NAME);
    if (NULL == m_hEventLog)
    {
        wprintf(L"RegisterEventSource failed with 0x%x.\n", GetLastError());
        DeregisterEventSource(m_hEventLog);
    }
}

WindowsLogger::~WindowsLogger()
{
    DeregisterEventSource(m_hEventLog);
}

void WindowsLogger::write(LogParam loggerDetails)
{
    if (m_hEventLog == NULL)
    {
        return;
    }
    LPCSTR insertStrings[2] = {NULL, NULL};
    insertStrings[0] = loggerDetails.message;
    if (!ReportEvent(m_hEventLog, eventLogType(loggerDetails.logType), loggerDetails.category, loggerDetails.eventID, NULL, 1, 0, insertStrings, NULL))
    {
        wprintf(L"ReportEvent failed with 0x%x for event 0x%x.\n", GetLastError(), MSG_BAD_FILE_CONTENTS);
        DeregisterEventSource(m_hEventLog);
    }
}

WORD WindowsLogger::eventLogType(LogType logType)
{
    WORD type = 0;

    switch (logType)
    {
    case 0:
        type = EVENTLOG_SUCCESS;
        break;
    case 1:
        type = EVENTLOG_ERROR_TYPE;
        break;
    case 2:
        type = EVENTLOG_INFORMATION_TYPE;
        break;
    case 3:
        type = EVENTLOG_WARNING_TYPE;
        break;
    default:
        break;
    }

    return type;
}