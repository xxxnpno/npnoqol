#include "HypixelAPI.h"

auto HypixelAPI::CheckKey() -> bool
{
    try
    {
        const std::string res = Network::Get(std::format("/v2/key?key={}", apiKey));
        const nlohmann::json jsonResponse = nlohmann::json::parse(res, nullptr, false);
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
        const std::string res = Network::Get(std::format("/player?key={}&name={}", apiKey, playerName));
        const nlohmann::json jsonResponse = nlohmann::json::parse(res->body, nullptr, false);

        return jsonResponse;
    }
    catch (...)
    {
        return nlohmann::json{};
    }
}