
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include <string>
#include <list>
#include <thread>
#include <chrono>
#include <tchar.h>

#include <nlohmann/json.hpp>

#include "ConfigurationHandler/ConfigurationHandler.h"
#include "Application.h"
#include "Timer.h"

#include <MeldCX_provider.h> //For event id's
#include <ApplicationErrorEventHandler.h>
#include <ApplicationHangEventHandler.h>
#include <ApplicationHandler.h>

#include <Logger.h>
#include <LoggerImpl.h> //Note: for LogType and LogParam

#define length(array) ((sizeof(array)) / (sizeof(array[0])))

using namespace std;
using json = nlohmann::json;
using namespace std::chrono_literals;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono;

void configWatcher()
{

    ConfigurationHandler::instance()->setFilename("configuration.json");
    ApplicationHandler appHandler;
    while (1)
    {
        ConfigurationHandler::instance()->update();

        if (ConfigurationHandler::instance()->isDirty())
        {
            int app_cnt = ConfigurationHandler::instance()->applications().size();

            list<Application>::iterator it = ConfigurationHandler::instance()->applications().begin();

            for (const Application &application : ConfigurationHandler::instance()->applications())
            {

                if (!appHandler.isProcessRunning(application))
                {
                    char message[100];
                    sprintf(message, "Running application %s \r\n", application.executableName().c_str());
                    Logger logger;
                    LogParam logParam = {
                        INFORMATION_LOG,
                        message,
                        MSG_RUN_APPLICATION,
                        UI_CATEGORY};

                    logger.write(logParam);
                    appHandler.runApp(application);
                }
            }
        }
    }
}

void periodicCheck()
{
#ifdef ENABLE_DEBUG
    wprintf(L"Checking...\r\n");
#endif

    ApplicationHandler appHandler;
    for (const Application &application : ConfigurationHandler::instance()->applications())
    {
        if (!appHandler.isProcessRunning(application))
        {
            char message[100];
            sprintf(message, "Running again the closed application %s due to user terminate it manually\r\n", application.executableName().c_str());
            Logger logger;
            LogParam logParam = {
                INFORMATION_LOG,
                message,
                MSG_RUN_APPLICATION,
                UI_CATEGORY};

            logger.write(logParam);

            appHandler.runApp(application);
        }
    }
}

void systemEvent(const wchar_t *event, const wchar_t *app)
{
#ifdef ENABLE_DEBUG
    wprintf(L"systemEvent: Event:%ls, App:%ls\r\n", event, app);
#endif

    ApplicationHandler appHandler;
    char app_char[50];
    wcstombs(app_char, app, 50);
    for (const Application &application : ConfigurationHandler::instance()->applications())
    {

        if (std::strcmp(application.executableName().c_str(), app_char) == 0)
        {
#ifdef ENABLE_DEBUG
            wprintf(L"App match: %ls\r\n", app);
#endif

            if (wcscmp(event, L"Error") == 0)
            {
#ifdef ENABLE_DEBUG
                wprintf(L"Error event...\r\n");
#endif

                char trial = 50;
                while (appHandler.isProcessRunning(application) && trial > 0)
                {
                    sleep_for(1000ms);
                    trial--;
#ifdef ENABLE_DEBUG
                    wprintf(L"Try again...\r\n");
#endif
                }

                if (trial > 0)
                {
                    int result = appHandler.runApp(application);
                    char message[100];
                    Logger logger;
                    if (result == 0)
                    {
                        sprintf(message, "Running again the closed application %s due to system event\r\n", application.executableName().c_str());

                        LogParam logParam = {
                            INFORMATION_LOG,
                            message,
                            MSG_RUN_APPLICATION,
                            UI_CATEGORY};

                        logger.write(logParam);
                    }
                    else
                    {
                        sprintf(message, "Error running the application %s due to error code %d\r\n", application.executableName().c_str(), result);

                        LogParam logParam = {
                            ERROR_LOG,
                            message,
                            MSG_RUN_APPLICATION,
                            UI_CATEGORY};

                        logger.write(logParam);
                    }
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{

    thread watchdogThread(configWatcher);
    ApplicationErrorEventHandler appErrEvtHandler;
    appErrEvtHandler.callback(systemEvent);
    ApplicationHangEventHandler appHangEvtHandler;
    appHangEvtHandler.callback(systemEvent);
    Timer timer;
    timer.setInterval(periodicCheck, 60000); // Check every minute
    appErrEvtHandler.run();
    appHangEvtHandler.run();
    watchdogThread.join();

    return 0;
}