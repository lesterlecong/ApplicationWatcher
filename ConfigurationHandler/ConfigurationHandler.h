#ifndef CONFIGURATIONHANDLER_H
#define CONFIGURATIONHANDLER_H

#include <string>
#include <filesystem>
#include <chrono>
#include <list>

#include "Application.h"

using namespace std;

/**
 * @brief This class handles the configuration file.
 *        These includes reading and parsing the configuration file.
 *        Currently it only supports json file.
 *        It was design as Singleton pattern so that we will have one copy of the applications
 *        to be monitored.
 *
 */
class ConfigurationHandler
{
public:
    /**
     * @brief This will return the instance of ConfigurationHandler class.
     *
     * @return ConfigurationHandler*
     */
    static ConfigurationHandler *instance();

    /**
     * @brief If need to set filename of the configuration file, this method can be invoked instead
     *        of the instance above.
     *
     * @param filename -holds the path or name of the configuration file.
     * @return ConfigurationHandler*
     */
    static ConfigurationHandler *instance(std::string filename);

    /**
     * @brief Set the Filename or path of the configuration file.
     *
     * @param filename -holds the path or name of the configuration file.
     */
    void setFilename(std::string filename);

    /**
     * @brief This will check if there are changes in the configuration file set on he setFilename method.
     *
     */
    void update();

    /**
     * @brief This will check if the lists of application are newly updated.
     *
     * @return true - there are changes on the list.
     * @return false - there are no changes on the list.
     */
    bool isDirty();

    /**
     * @brief Return the list of applications found on the configuration file.
     *
     * @return list<Application>
     */
    list<Application> applications();

private:
    void parseFile();

    ConfigurationHandler();
    ConfigurationHandler(std::string filename);
    ~ConfigurationHandler();

    static ConfigurationHandler *m_instance;
    list<Application> m_applications;
    string m_filename;
    filesystem::file_time_type m_last_write_time;
    bool m_isDirty;
};

#endif