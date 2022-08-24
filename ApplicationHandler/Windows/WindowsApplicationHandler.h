#ifndef WINDOWSAPPLICATIONHANDLER_H
#define WINDOWSAPPLICATIONHANDLER_H
#include "ApplicationHandlerImpl.h"

/**
 * @brief This class handles the checking and running of the application in Windows OS.
 *
 */
class WindowsApplicationHandler : public ApplicationHandlerImpl
{
public:
    WindowsApplicationHandler();

    /**
     * @brief Checking the application if it is running or not
     *
     * @param app - holds the information of the application to check if it is running.
     * @return true - if the application is currently running or active.
     * @return false  - if currently closed.
     */
    virtual bool isProcessRunning(Application app);

    /**
     * @brief Perform to run the executable file.
     *
     * @param app - holds the information of the application to run.
     * @return int - if values are below or equal to 33 it means error except 0 which means success.
     *               Above 33 will be the HANDLE instance value.
     */
    virtual int runApp(Application app);
};
#endif