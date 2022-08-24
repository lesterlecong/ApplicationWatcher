#ifndef WINDOWSEVENTHANDLER_H
#define WINDOWSEVENTHANDLER_H
#ifdef _WIN32

#ifndef UNICODE
#define UNICODE
#endif
#define WIN32_LEAN_AND_MEAN

//#define LOAD_LIBRARY_AS_IMAGE_RESOURCE 0x00000020
#define NOTIFICATION_EVENT 1
#define KEYBOARD_EVENT 0

#include <EventHandler.h>
#ifndef WINDOWS_INCLUDE
#define WINDOWS_INCLUDE
#include <windows.h>
#endif
#include <stdio.h>
#include <ctype.h>
#include <wchar.h>
#include <vector>

/**
 * @brief This the parent class for Windows event handler.
 *        It is the responsible for listening the events.
 *        The derived classes are needed to implement the parse and supply the provider name and library.
 *
 */
class WindowsEventHandler : public EventHandler
{
public:
    WindowsEventHandler();
    virtual ~WindowsEventHandler();

protected:
    virtual void runProcess();
    virtual void parse(LPWSTR event, LPWSTR message) = 0;
    void setProvider(const wchar_t *providerName);
    void setResourceDLL(const wchar_t *resourceDLL);
    std::vector<wstring> splitManyW(const wstring &original, const wstring &delimiters);

    HANDLE GetMessageResources();
    DWORD SeekToLastRecord(HANDLE hEventLog);
    DWORD GetLastRecordNumber(HANDLE hEventLog, DWORD *pdwRecordNumber);
    DWORD ReadRecord(HANDLE hEventLog, PBYTE &pBuffer, DWORD dwRecordNumber, DWORD dwFlags);
    DWORD DumpNewRecords(HANDLE hEventLog);
    DWORD GetEventTypeName(DWORD EventType);

    LPWSTR GetMessageString(DWORD MessageId, DWORD argc, LPWSTR argv);

    const wchar_t *m_resourceDLL;
    const wchar_t *m_providerName;
    HANDLE m_hResources;
    LPWSTR pEventTypeNames[5];
};
#endif
#endif