#include "Logger.h"
#include "LoggerImpl.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <WindowsLogger.h>
#define LOGGER_IMPL WindowsLogger
#elif __linux__
// pass
#endif

Logger::Logger()
{
    m_loggerImpl = new LOGGER_IMPL();
}

Logger::~Logger()
{
    delete m_loggerImpl;
}

void Logger::write(LogParam loggerDetails)
{
    m_loggerImpl->write(loggerDetails);
}