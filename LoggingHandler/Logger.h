#ifndef LOGGER_H
#define LOGGER_H

#include "LoggerImpl.h"

class Logger
{
public:
    Logger();
    ~Logger();
    void write(LogParam loggerDetails);

private:
    LoggerImpl *m_loggerImpl;
};

#endif