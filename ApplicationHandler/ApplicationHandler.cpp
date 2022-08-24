#include "ApplicationHandler.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <WindowsApplicationHandler.h>
#define APPLICATIONHANDLER_IMPL WindowsApplicationHandler
#elif __linux__
// pass
#endif

ApplicationHandler::ApplicationHandler()
{
    m_appHandlerImpl = new APPLICATIONHANDLER_IMPL();
}

ApplicationHandler::~ApplicationHandler()
{
    if (m_appHandlerImpl)
    {
        delete m_appHandlerImpl;
    }
}

bool ApplicationHandler::isProcessRunning(Application app)
{
    if (m_appHandlerImpl)
    {
        return m_appHandlerImpl->isProcessRunning(app);
    }

    return false;
}
int ApplicationHandler::runApp(Application app)
{
    if (m_appHandlerImpl)
    {
        return m_appHandlerImpl->runApp(app);
    }

    return -1;
}