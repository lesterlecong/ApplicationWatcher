#include "WindowsApplicationHandler.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include <string>
#include <list>
#ifndef WINDOWS_INCLUDE
#define WINDOWS_INCLUDE
#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#endif
#include <map>
#include <thread>
#include <chrono>
#include <tchar.h>
#include <WinBase.h>
#include <Application.h>

#ifndef UNICODE
#define UNICODE
#endif
#define WIN32_LEAN_AND_MEAN

using namespace std;

WindowsApplicationHandler::WindowsApplicationHandler()
{
}

bool WindowsApplicationHandler::isProcessRunning(Application app)
{
#ifdef ENABLE_DEBUG
    wprintf(L"WindowsApplicationHandler::isProcessRunning: checking app: %ls\r\n", app.executableName().c_str());
#endif

    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (!Process32First(snapshot, &entry))
    {
        CloseHandle(snapshot);
        return false;
    }

    do
    {
        if (strcmp(entry.szExeFile, app.executableName().c_str()) == 0)
        {
            CloseHandle(snapshot);
            return true;
        }
    } while (Process32Next(snapshot, &entry));

    CloseHandle(snapshot);
    return false;
}

int WindowsApplicationHandler::runApp(Application app)
{
    string exec_temp = app.executableName();
    string command_temp = app.command();
    string dir_temp = app.path();
    const char *exec_temp_str = exec_temp.c_str();
    const char *command_temp_str = command_temp.c_str();
    const char *directory_temp_str = dir_temp.c_str();

    LPCSTR command, directory;
    command = (strcmp(command_temp_str, "None") == 0) ? NULL : command_temp_str;
    directory = (strcmp(directory_temp_str, "None") == 0) ? NULL : directory_temp_str;

    INT_PTR exec_result = (INT_PTR)ShellExecute(NULL, "open", exec_temp_str, command, directory, SW_SHOWNORMAL);
    return (exec_result <= 33);
}