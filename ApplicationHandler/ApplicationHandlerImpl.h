#ifndef APPLICATIONHANDLERIMPL_H
#define APPLICATIONHANDLERIMPL_H

#include <Application.h>

class ApplicationHandlerImpl
{
public:
    virtual bool isProcessRunning(Application app) = 0;
    virtual int runApp(Application app) = 0;
};
#endif