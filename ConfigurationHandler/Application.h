#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>

class Application
{
public:
    Application();
    ~Application();

    void executableName(std::string sName);
    void path(std::string sPath);
    void command(std::string sCommand);
    void processID(int nPid);

    std::string executableName() const;
    std::string path() const;
    std::string command() const;

private:
    std::string m_executableName;
    std::string m_path;
    std::string m_command;
};

#endif