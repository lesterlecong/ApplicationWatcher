#ifndef LOGGERIMPL_H
#define LOGGERIMPL_H

typedef enum LogType_t
{
    SUCCESS_LOG,
    ERROR_LOG,
    INFORMATION_LOG,
    WARNING_LOG

} LogType;

typedef struct LogParam_t
{
    LogType logType;
    char *message;
    int eventID;
    int category;
} LogParam;

/**
 * @brief This class will be abstract class for logging handlers.
 *        It only supports writing the log into the system.
 *
 */
class LoggerImpl
{
public:
    virtual void write(LogParam loggerDetails) = 0;
};

#endif