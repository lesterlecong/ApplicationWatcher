#ifndef LOGGER_H
#define LOGGER_H

#include "LoggerImpl.h"

/**
 * @brief This class will be the interface for the logging.
 *
 */
class Logger
{
public:
    Logger();
    ~Logger();

    /**
     * @brief This is the interface for writing the logs on the system.
     *
     * @param loggerDetails - holds the details or parameters of the log.
     */
    void write(LogParam loggerDetails);

private:
    LoggerImpl *m_loggerImpl;
};

#endif