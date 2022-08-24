#include "ApplicationErrorEventHandler.h"
#include <WindowsEventHandler.h>
#include <wchar.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using std::vector;
using std::wstring;

ApplicationErrorEventHandler::ApplicationErrorEventHandler()
{
    setProvider(L"Application Error");
    setResourceDLL(L"C:/Windows/System32/wer.dll");
}

void ApplicationErrorEventHandler::parse(LPWSTR event, LPWSTR message)
{

    wstring separators = L",\n";
    vector<wstring> results = splitManyW(message, separators);

    if (results.size())
    {
        vector<wstring> app = splitManyW(results[0], L":\n");
        if (m_callback)
        {
            app[1].erase(remove_if(app[1].begin(), app[1].end(), ::isspace), app[1].end());
            m_callback(event, app[1].c_str());
        }
    }
}