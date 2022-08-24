#ifndef CONFIGURATIONHANDLER_H
#define CONFIGURATIONHANDLER_H

#include <string>
#include <filesystem>
#include <chrono>
#include <list>

#include "Application.h"

using namespace std;

class ConfigurationHandler
{
public:
    static ConfigurationHandler *instance();
    static ConfigurationHandler *instance(std::string filename);

    void setFilename(std::string filename);
    void update();
    bool isDirty();
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