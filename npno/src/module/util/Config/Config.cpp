#include "Config.h"

#include <print>
#include <fstream>

#include <nlohmann/json.hpp>

std::string Config::GetHypixelAPIKey()
{
    std::ifstream file("config.json");
    if (!file.is_open())
    {
        std::ofstream outFile("config.json");
        nlohmann::json defaultConfig = {
            {"api", {
                {"hypixel", ""}
            }}
        };
        outFile << defaultConfig.dump(4);
        outFile.close();
        return "";
    }

    std::string key;
    try
    {
        nlohmann::json json = nlohmann::json::parse(file);
        key = json["api"]["hypixel"].get<std::string>();
    }
    catch (...)
    {
        return "";
    }

    return key;
}