#ifndef APPLICATIONHANDLER_H
#define APPLICATIONHANDLER_H

#include "ApplicationHandlerImpl.h"
#include <Application.h>

class ApplicationHandler
{
public:
    ApplicationHandler();
    ~ApplicationHandler();

    bool isProcessRunning(Application app);
    int runApp(Application app);

private:
    ApplicationHandlerImpl *m_appHandlerImpl;
};

#endif