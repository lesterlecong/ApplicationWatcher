#ifndef APPLICATIONHANDLERIMPL_H
#define APPLICATIONHANDLERIMPL_H

#include <Application.h>

/**
 * @brief This is the abstract class for Application handling.
 *
 */
class ApplicationHandlerImpl
{
public:
    /**
     * @brief This is a pure virtual method that required by child classes to override.
     *        Used to check if the application is currently running.
     *
     * @param app - holds the information of the application to check if it is running.
     * @return true - if the current application is running.
     * @return false - if the current application is not running.
     */
    virtual bool isProcessRunning(Application app) = 0;

    /**
     * @brief This is a pure virtual method that required by child classes to override.
     *        Used to run the application in the system.
     *
     * @param app - holds the information of the application to run.
     * @return int - if values are below or equal to 33 it means error except 0 which means success.
     *               Above 33 will be the HANDLE instance value.
     */
    virtual int runApp(Application app) = 0;
};
#endif