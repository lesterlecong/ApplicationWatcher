#ifndef APPLICATIONHANDLER_H
#define APPLICATIONHANDLER_H

#include "ApplicationHandlerImpl.h"
#include <Application.h>

/**
 * @brief This class will be the interface for the handling of application checking and running.
 *
 */
class ApplicationHandler
{
public:
    ApplicationHandler();
    ~ApplicationHandler();

    /**
     * @brief This is the interface to check if the current application is running on the system.
     *
     * @param app - holds the information of the application to check if it is running.
     * @return true - if the current application is running.
     * @return false - if the current application is not running.
     */
    bool isProcessRunning(Application app);

    /**
     * @brief This is the interface to perform the running of the application.
     *
     * @param app - holds the information of the application to run in the system
     * @return int - if values are below or equal to 33 it means error except 0 which means success.
     *               Above 33 will be the HANDLE instance value.
     */
    int runApp(Application app);

private:
    ApplicationHandlerImpl *m_appHandlerImpl;
};

#endif