#include "HypixelAPI.h"
#include <print>

auto HypixelAPI::CheckKey() -> bool
{
    try
    {
        httplib::Result res = cli.Get(std::format("/v2/key?key={}", apiKey));
        if (!res or res->status != 200)
        {
            return false;
        }

        nlohmann::json jsonResponse = nlohmann::json::parse(res->body, nullptr, false);
        if (jsonResponse["cause"] == "Invalid API key")
        {
            return false;
        }

        return true;
    }
    catch (...)
    {
        return false;
    }
}

auto HypixelAPI::IsNicked(const nlohmann::json& json) -> bool
{
    try
    {
        if (json["player"].is_null())
        {
            return true;
        }

        return false;
    }
    catch(...)
    {
        return false;
    }
}

auto HypixelAPI::GetPlayerStats(const std::string& playerName) -> nlohmann::json
{
    try
    {
        httplib::Result res = cli.Get(std::format("/player?key={}&name={}", apiKey, playerName));
        if (!res or res->status != 200)
        {
            return nlohmann::json{};
        }

        nlohmann::json jsonResponse = nlohmann::json::parse(res->body, nullptr, false);
        return jsonResponse;
    }
    catch (...)
    {
        return nlohmann::json{};
    }
}