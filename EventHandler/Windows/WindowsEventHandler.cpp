#include "WindowsEventHandler.h"
#include <stdlib.h>
#include <vector>
#include <wchar.h>
#include <iostream>
#include <sstream>
#include <string>

WindowsEventHandler::WindowsEventHandler()
{
    m_hResources = NULL;
    pEventTypeNames[0] = L"Error";
    pEventTypeNames[1] = L"Warning";
    pEventTypeNames[2] = L"Informational";
    pEventTypeNames[3] = L"Audit Success";
    pEventTypeNames[4] = L"Audit Failure";

    m_abortRequested.store(false);
}

WindowsEventHandler::~WindowsEventHandler()
{
    abortAndJoin();
}

void WindowsEventHandler::runProcess()
{
    m_isRunning.store(true);

    HANDLE hEventLog = NULL;
    // HANDLE aWaitHandles[2];
    HANDLE aWaitHandle;
    DWORD status = ERROR_SUCCESS;
    DWORD dwWaitReason = 0;
    DWORD dwLastRecordNumber = 0;

    // Get the DLL that contains the message table string resources for the provider.
    m_hResources = GetMessageResources();
    if (NULL == m_hResources)
    {
        wprintf(L"GetMessageResources failed.\n");
        goto cleanup;
    }

    aWaitHandle = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (NULL == aWaitHandle)
    {
        wprintf(L"CreateEvent failed with %lu.\n", GetLastError());
        goto cleanup;
    }

    // Open the log file. The source name (provider) must exist as
    // a subkey of Application.
    hEventLog = OpenEventLog(NULL, m_providerName);
    if (NULL == hEventLog)
    {
        wprintf(L"OpenEventLog failed with 0x%x.\n", GetLastError());
        goto cleanup;
    }

    // Seek to the last record in the event log and read it in order
    // to position the cursor for reading any new records when the
    // service notifies you that new records have been written to the
    // log file.
    status = SeekToLastRecord(hEventLog);
    if (ERROR_SUCCESS != status)
    {
        wprintf(L"SeekToLastRecord failed with %lu.\n", status);
        goto cleanup;
    }

    // Request notification when events are written to the log.
    if (!NotifyChangeEventLog(hEventLog, aWaitHandle))
    {
        wprintf(L"NotifyChangeEventLog failed with %lu.\n", GetLastError());
        goto cleanup;
    }

    wprintf(L"Waiting for notification of new events (press any key to quit)...\n\n");

    // Loop until the user presses a key or there is an error.
    while (false == m_abortRequested.load())
    {
        // wprintf(L"I'm running dude.Provider: %ls!\r\n", m_providerName);

        dwWaitReason = WaitForSingleObject(aWaitHandle, INFINITE);
        // wprintf(L"Wait reason: %lu\r\n", dwWaitReason);

        if (dwWaitReason == 0) // Notification results
        {
            // wprintf(L"Notification event!\r\n");
            if (ERROR_SUCCESS != (status = DumpNewRecords(hEventLog)))
            {
                wprintf(L"DumpNewRecords failed.\n");
                m_abortRequested.store(true);
            }

            wprintf(L"\nWaiting for notification of new events (press any key to quit)...\n");
            ResetEvent(aWaitHandle);
        }
        else
        {
            if (WAIT_FAILED == dwWaitReason)
            {
                wprintf(L"WaitForSingleObject failed with %lu\n", GetLastError());
                m_abortRequested.store(true);
            }
        }

        // wprintf(L"Is aborted? %s\r\n", (m_abortRequested.load()) ? "Yes" : "No");
    }

cleanup:

    if (hEventLog)
        CloseEventLog(hEventLog);

    if (aWaitHandle)
        CloseHandle(aWaitHandle);

    m_isRunning.store(false);
}

void WindowsEventHandler::setProvider(const wchar_t *providerName)
{
    m_providerName = providerName;
}

void WindowsEventHandler::setResourceDLL(const wchar_t *resourceDLL)
{
    m_resourceDLL = resourceDLL;
}

HANDLE WindowsEventHandler::GetMessageResources()
{
    HANDLE hResources = NULL;
    hResources = LoadLibraryEx(m_resourceDLL, NULL, LOAD_LIBRARY_AS_IMAGE_RESOURCE | LOAD_LIBRARY_AS_DATAFILE);
    if (NULL == hResources)
    {
        wprintf(L"LoadLibrary failed with %lu.\n", GetLastError());
    }
    else
    {
#ifdef ENABLE_DEBUG
        wprintf(L"LoadLibrary success!!!\r\n");
#endif
    }

    return hResources;
}

DWORD WindowsEventHandler::SeekToLastRecord(HANDLE hEventLog)
{
    DWORD status = ERROR_SUCCESS;
    DWORD dwLastRecordNumber = 0;
    PBYTE pRecord = NULL;

    status = GetLastRecordNumber(hEventLog, &dwLastRecordNumber);

    if (ERROR_SUCCESS != status)
    {
#ifdef ENABLE_DEBUG
        wprintf(L"Get LastRecorNumber failed.%lu\r\n", GetLastError());
#endif
        if (pRecord)
        {
            free(pRecord);
        }
    }

    status = ReadRecord(hEventLog, pRecord, dwLastRecordNumber, EVENTLOG_SEEK_READ | EVENTLOG_FORWARDS_READ);
    if (ERROR_SUCCESS != status)
    {
        wprintf(L"ReadRecord failed seeking to record %lu.\n", dwLastRecordNumber);
        if (pRecord)
        {
            free(pRecord);
        }
    }

    return status;
}

DWORD WindowsEventHandler::GetLastRecordNumber(HANDLE hEventLog, DWORD *pdwRecordNumber)
{
    DWORD status = ERROR_SUCCESS;
    DWORD oldestRecordNumber = 0;
    DWORD numberOfRecords = 0;

    if (!GetOldestEventLogRecord(hEventLog, &oldestRecordNumber))
    {
        wprintf(L"GetOldestEventLogRecord failed with %lu.\n", status = GetLastError());
        return status;
    }

    if (!GetNumberOfEventLogRecords(hEventLog, &numberOfRecords))
    {
        wprintf(L"GetOldestEventLogRecord failed with %lu.\n", status = GetLastError());
        return status;
    }

    *pdwRecordNumber = oldestRecordNumber + numberOfRecords - 1;
}

DWORD WindowsEventHandler::ReadRecord(HANDLE hEventLog, PBYTE &pBuffer, DWORD dwRecordNumber, DWORD dwFlags)
{
    DWORD status = ERROR_SUCCESS;
    DWORD dwBytesToRead = sizeof(EVENTLOGRECORD);
    DWORD dwBytesRead = 0;
    DWORD dwMinimumBytesToRead = 0;
    PBYTE pTemp = NULL;

    pBuffer = (PBYTE)malloc(sizeof(EVENTLOGRECORD));

    if (!ReadEventLog(hEventLog, dwFlags, dwRecordNumber, pBuffer, dwBytesToRead, &dwBytesRead, &dwMinimumBytesToRead))
    {
        status = GetLastError();
        if (ERROR_INSUFFICIENT_BUFFER == status)
        {
            status = ERROR_SUCCESS;

            pTemp = (PBYTE)realloc(pBuffer, dwMinimumBytesToRead);
            if (NULL == pTemp)
            {
                wprintf(L"Failed to reallocate memory for the record buffer (%d bytes).\n", dwMinimumBytesToRead);
                return status;
            }

            pBuffer = pTemp;

            dwBytesToRead = dwMinimumBytesToRead;

            if (!ReadEventLog(hEventLog, dwFlags, dwRecordNumber, pBuffer, dwBytesToRead, &dwBytesRead, &dwMinimumBytesToRead))
            {
                wprintf(L"Second ReadEventLog failed with %lu.\n", status = GetLastError());
                return status;
            }
        }
        else
        {
            if (ERROR_HANDLE_EOF != status)
            {
                wprintf(L"ReadEventLog failed with %lu.\n", status);
                return status;
            }
        }
    }
}

DWORD WindowsEventHandler::DumpNewRecords(HANDLE hEventLog)
{
    DWORD status = ERROR_SUCCESS;
    DWORD dwLastRecordNumber = 0;
    LPWSTR pMessage = NULL;
    LPWSTR pFinalMessage = NULL;
    PBYTE pRecord = NULL;

    status = ReadRecord(hEventLog, pRecord, 0, EVENTLOG_SEQUENTIAL_READ | EVENTLOG_FORWARDS_READ);
    if (ERROR_SUCCESS != status && ERROR_HANDLE_EOF != status)
    {
        wprintf(L"ReadRecord (priming read) failed.\n");
        if (pRecord)
            free(pRecord);

        return status;
    }

    while (ERROR_HANDLE_EOF != status)
    {
        // wprintf(L"DumpNewRecords:: providerName: %ls, pRecord: %ls\r\n", m_providerName, (LPWSTR)(pRecord + sizeof(EVENTLOGRECORD)));
        if (0 == wcscmp(m_providerName, (LPWSTR)(pRecord + sizeof(EVENTLOGRECORD))))
        {
            wprintf(L"record number: %lu\n", ((PEVENTLOGRECORD)pRecord)->RecordNumber);
            wprintf(L"status code: %d\n", ((PEVENTLOGRECORD)pRecord)->EventID & 0xFFFF);
            DWORD eventType = GetEventTypeName(((PEVENTLOGRECORD)pRecord)->EventType);
            LPWSTR eventName = pEventTypeNames[eventType];
            wprintf(L"event type: %S\n", eventName);

            pMessage = GetMessageString(((PEVENTLOGRECORD)pRecord)->EventCategory, 0, NULL);
            if (pMessage)
            {
                wprintf(L"event category: %S\r\n", pMessage);
                LocalFree(pMessage);
                pMessage = NULL;
            }

            pMessage = GetMessageString(((PEVENTLOGRECORD)pRecord)->EventID,
                                        ((PEVENTLOGRECORD)pRecord)->NumStrings, (LPWSTR)(pRecord + ((PEVENTLOGRECORD)pRecord)->StringOffset));
            if (pMessage)
            {

                parse(eventName, pMessage);
                LocalFree(pMessage);
                pMessage = NULL;
            }

            if (((PEVENTLOGRECORD)pRecord)->DataLength > 0)
            {
                wprintf(L"event data: %s\n", (LPWSTR)(pRecord + ((PEVENTLOGRECORD)pRecord)->DataOffset));
            }

            wprintf(L"\n");
        }

        status = ReadRecord(hEventLog, pRecord, 0, EVENTLOG_SEQUENTIAL_READ | EVENTLOG_FORWARDS_READ);
        if (ERROR_SUCCESS != status && ERROR_HANDLE_EOF != status)
        {
            wprintf(L"ReadRecord sequential failed.\n");
            if (pRecord)
                free(pRecord);

            return status;
        }
    }

    if (ERROR_HANDLE_EOF == status)
    {
        status = ERROR_SUCCESS;
    }

    if (pRecord)
        free(pRecord);

    return status;
}

DWORD WindowsEventHandler::GetEventTypeName(DWORD EventType)
{
    DWORD index = 0;

    switch (EventType)
    {
    case EVENTLOG_ERROR_TYPE:
        index = 0;
        break;
    case EVENTLOG_WARNING_TYPE:
        index = 1;
        break;
    case EVENTLOG_INFORMATION_TYPE:
        index = 2;
        break;
    case EVENTLOG_AUDIT_SUCCESS:
        index = 3;
        break;
    case EVENTLOG_AUDIT_FAILURE:
        index = 4;
        break;
    }

    return index;
}

LPWSTR WindowsEventHandler::GetMessageString(DWORD MessageId, DWORD argc, LPWSTR argv)
{
    LPWSTR pMessage = NULL;
    DWORD dwFormatFlags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_ALLOCATE_BUFFER;
    DWORD_PTR *pArgs = NULL;
    LPWSTR pString = argv;

    if (argc > 0)
    {
        pArgs = (DWORD_PTR *)malloc(sizeof(DWORD_PTR) * argc);
        if (pArgs)
        {
            dwFormatFlags |= FORMAT_MESSAGE_ARGUMENT_ARRAY;

            for (DWORD i = 0; i < argc; i++)
            {
                pArgs[i] = (DWORD_PTR)pString;
                pString += wcslen(pString) + 1;
            }
        }
        else
        {
            dwFormatFlags |= FORMAT_MESSAGE_IGNORE_INSERTS;
            wprintf(L"Failed to allocate memory for the insert string array.\n");
        }
    }
    if (!FormatMessage(dwFormatFlags,
                       m_hResources,
                       MessageId,
                       0,
                       (LPWSTR)&pMessage,
                       0,
                       (va_list *)pArgs))
    {
        wprintf(L"Format message failed with %lu\n", GetLastError());
    }

    if (pArgs)
        free(pArgs);

    return pMessage;
}

std::vector<wstring> WindowsEventHandler::splitManyW(const wstring &original, const wstring &delimiters)
{
    std::wstringstream stream(original);
    std::wstring line;
    vector<wstring> wordVector;

    while (std::getline(stream, line))
    {
        std::size_t prev = 0, pos;
        while ((pos = line.find_first_of(delimiters, prev)) != std::wstring::npos)
        {
            if (pos > prev)
                wordVector.emplace_back(line.substr(prev, pos - prev));

            prev = pos + 1;
        }

        if (prev < line.length())
            wordVector.emplace_back(line.substr(prev, std::wstring::npos));
    }

    return wordVector;
}