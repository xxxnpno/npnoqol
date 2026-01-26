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

auto Config::SetHypixelAPIKey(const std::string& apiKey) -> void
{
    nlohmann::json config;

    std::ifstream inFile("config.json");
    if (inFile.is_open())
    {
        try
        {
            config = nlohmann::json::parse(inFile);
        }
        catch (...)
        {
            config = {
                {"api", {
                    {"hypixel", ""}
                }}
            };
        }
        inFile.close();
    }
    else
    {
        config = {
            {"api", {
                {"hypixel", ""}
            }}
        };
    }

    config["api"]["hypixel"] = apiKey;

    std::ofstream outFile("config.json");
    if (!outFile.is_open())
    {
        return;
    }

    outFile << config.dump(4);
    outFile.close();
}