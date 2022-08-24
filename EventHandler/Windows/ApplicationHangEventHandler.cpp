#include "ApplicationHangEventHandler.h"
#include <WindowsEventHandler.h>
#include <wchar.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using std::vector;
using std::wstring;

ApplicationHangEventHandler::ApplicationHangEventHandler()
{
    setProvider(L"Application Hang");
    setResourceDLL(L"C:/Windows/System32/wersvc.dll");
}

void ApplicationHangEventHandler::parse(LPWSTR event, LPWSTR message)
{
    wstring separators = L" \n";

    vector<wstring> results = splitManyW(message, separators);

    if (results.size())
    {
        if (m_callback)
        {
            results[2].erase(remove_if(results[2].begin(), results[2].end(), ::isspace), results[2].end());
            m_callback(event, results[2].c_str());
        }
    }
}