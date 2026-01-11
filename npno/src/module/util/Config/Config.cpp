#include "Config.h"

#include <fstream>

#include <nlohmann/json.hpp>

std::string Config::GetHypixelAPIKey()
{
    std::string apiKey;

    std::ifstream file("config.json");
    nlohmann::json json;
    file >> json;

    apiKey = json["api"]["hypixel"].get<std::string>();

    return apiKey;
}