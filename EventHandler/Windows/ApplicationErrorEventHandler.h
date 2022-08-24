#ifndef APPLICATIONERROREVENTHANDLER_H
#define APPLICATIONERROREVENTHANDLER_H

#ifdef _WIN32

#include <WindowsEventHandler.h>

class ApplicationErrorEventHandler : public WindowsEventHandler
{
public:
    ApplicationErrorEventHandler();

protected:
    virtual void parse(LPWSTR event, LPWSTR message);
};

#endif
#endif