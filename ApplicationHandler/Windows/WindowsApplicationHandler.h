#ifndef WINDOWSAPPLICATIONHANDLER_H
#define WINDOWSAPPLICATIONHANDLER_H
#include "ApplicationHandlerImpl.h"

class WindowsApplicationHandler : public ApplicationHandlerImpl
{
public:
    WindowsApplicationHandler();
    virtual bool isProcessRunning(Application app);
    virtual int runApp(Application app);
};
#endif