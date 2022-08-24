#ifndef WINDOWSAPPLICATIONHANDLER_H
#define WINDOWSAPPLICATIONHANDLER_H
#include "ApplicationHandlerImpl.h"

class WindowsApplicationHandler : public ApplicationHandlerImpl
{
public:
    WindowsApplicationHandler();

    /**
     * @brief Checking the application if it is running or not
     *
     * @param app - Application object that is listed on the configuration file.
     * @return true - if the application is currently running or active.
     * @return false  - if currently closed.
     */
    virtual bool isProcessRunning(Application app);

    /**
     * @brief Perform to run the executable file.
     *
     * @param app - holds the executable name to run.
     * @return int - if values are below or equal to 33 it means error except 0 which means success.
     *               Above 33 will be the HANDLE instance value.
     */
    virtual int runApp(Application app);
};
#endif