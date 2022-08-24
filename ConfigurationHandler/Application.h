#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>

/**
 * @brief This class holds the information of an Application's path, executable name,
 *        and its paramaters to be used during startup.
 *
 */
class Application
{
public:
    Application();
    ~Application();

    /**
     * @brief Receives the name of the executable file.
     *        It should be match on the actual name of the application or else it will not run.
     *
     * @param sName - holds the name of the executable file.
     */
    void executableName(std::string sName);

    /**
     * @brief Receives the path of the application.
     *        Note: The path should be match on actual where the application is located.
     *              This is case sensitive.
     *
     * @param sPath - holds the path of the application
     */
    void path(std::string sPath);

    /**
     * @brief Receives the paramaters or commands to be used during startup of the application.
     *
     * @param sCommand - holds the command/parameter to be used during startup.
     */
    void command(std::string sCommand);

    /**
     * @brief Returns the name of the executable file.
     *
     * @return std::string
     */
    std::string executableName() const;

    /**
     * @brief Returns the set path of the application.
     *
     * @return std::string
     */
    std::string path() const;

    /**
     * @brief Returns the command/parameter set.
     *
     * @return std::string
     */
    std::string command() const;

private:
    std::string m_executableName;
    std::string m_path;
    std::string m_command;
};

#endif