#ifndef APPLICATION_HANG_H
#define APPLICATION_HANG_H
#ifdef _WIN32

#include <WindowsEventHandler.h>

class ApplicationHangEventHandler : public WindowsEventHandler
{
public:
    ApplicationHangEventHandler();

protected:
    virtual void parse(LPWSTR event, LPWSTR message);
};

#endif
#endif