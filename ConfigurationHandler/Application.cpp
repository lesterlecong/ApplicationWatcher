#include "Application.h"

using namespace std;

Application::Application()
{
    m_executableName = "";
    m_command = "";
    m_path = "";
}

Application::~Application()
{
}

void Application::executableName(string sName)
{
    m_executableName = sName;
}

void Application::path(string sPath)
{
    m_path = sPath;
}

void Application::command(string sCommand)
{
    m_command = sCommand;
}

string Application::executableName() const
{
    return m_executableName;
}

string Application::path() const
{
    return m_path;
}

string Application::command() const
{
    return m_command;
}
