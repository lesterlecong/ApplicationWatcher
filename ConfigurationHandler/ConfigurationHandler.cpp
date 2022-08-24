#include "ConfigurationHandler.h"

#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <list>
#include <nlohmann/json.hpp>
#include "Application.h"

using namespace std;
using json = nlohmann::json;

ConfigurationHandler *ConfigurationHandler::m_instance = NULL;

ConfigurationHandler *ConfigurationHandler::instance()
{
    if (m_instance == NULL)
    {
        m_instance = new ConfigurationHandler();
    }

    return m_instance;
}

ConfigurationHandler *ConfigurationHandler::instance(std::string filename)
{
    if (m_instance == NULL)
    {
        m_instance = new ConfigurationHandler(filename);
    }

    return m_instance;
}

ConfigurationHandler::ConfigurationHandler()
{
}

ConfigurationHandler::ConfigurationHandler(std::string filename)
{
    m_filename = filename;
}

ConfigurationHandler::~ConfigurationHandler()
{
    if (m_applications.size())
    {
        m_applications.clear();
    }
}

void ConfigurationHandler::setFilename(std::string filename)
{
    m_filename = filename;
}

void ConfigurationHandler::update()
{
    auto current_last_write_time = std::filesystem::last_write_time(m_filename);
    if (m_last_write_time != current_last_write_time)
    {
        m_last_write_time = current_last_write_time;
        m_isDirty = true;
#ifdef ENABLE_DEBUG
        printf("File modified\r\n");
#endif
        parseFile();
    }
    else
    {
        m_isDirty = false;
    }
}

bool ConfigurationHandler::isDirty()
{
    return m_isDirty;
}

list<Application> ConfigurationHandler::applications()
{
    return m_applications;
}

void ConfigurationHandler::parseFile()
{

    ifstream in_file(m_filename);
    json data = json::parse(in_file);
#ifdef ENABLE_DEBUG
    cout << data.dump(4) << endl;
    cout << data.size() << endl;
#endif
    list<Application> applications;

    for (int app_cnt = 0; app_cnt < data.size(); app_cnt++)
    {
        try
        {
            Application application;

            application.executableName(data[app_cnt]["Executable"]);
            application.path(data[app_cnt]["Path"]);
            application.command(data[app_cnt]["Command"]);

            applications.push_back(application);
        }
        catch (const std::exception &e)
        {
            cout << e.what() << '\n';
        }
    }

    m_applications = applications;
}
