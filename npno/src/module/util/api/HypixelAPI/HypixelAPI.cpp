#include "HypixelAPI.h"

#include <print>

auto HypixelAPI::CheckKey() -> bool
{
    try
    {
        const std::string res = Network::Get(std::format("/v2/key?key={}", apiKey));
		if (nlohmann::json::accept(res))
        {
            const nlohmann::json jsonResponse = nlohmann::json::parse(res, nullptr, false);
            if (jsonResponse["cause"] == "Invalid API key")
            {
                return false;
            }
            return true;
        }
        return false;

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
        if (json.contains("player") and json["player"].is_null())
        {
            return true;
        }
    }
    catch (...)
    {
        return false;
    }

    return false;
}

auto HypixelAPI::GetPlayerStats(const std::string& playerName) -> nlohmann::json
{
    try
    {
        const std::string res = Network::Get(std::format("/player?key={}&name={}", apiKey, playerName));
        if (nlohmann::json::accept(res))
        {
            const nlohmann::json jsonResponse = nlohmann::json::parse(res, nullptr, false);

            return jsonResponse;
        }
        return nlohmann::json{};
    }
    catch (...)
    {
        return nlohmann::json{};
    }
}

auto HypixelAPI::AddNickPlayer(const std::string& playerName) -> void
{
    Nick player{};
    player.warned = false;

    nickList.insert({ playerName, player });
}

auto HypixelAPI::GetNickList() -> std::map<std::string, Nick>&
{
    return nickList;
}

auto HypixelAPI::AddAutoGGLine(const std::string& line) -> void
{
    autoGGLines.push_back(line);
}

auto HypixelAPI::GetAutoGGLines() -> std::vector<std::string>
{
    return autoGGLines;
}

auto HypixelAPI::GetCurrentGamemode() -> HypixelGamemode::Gamemode
{
    return currentGamemode;
}

auto HypixelAPI::SetCurrentGamemode(const HypixelGamemode::Gamemode gamemode) -> void
{
    currentGamemode = gamemode;
}

auto HypixelAPI::GetCurrentMode() -> std::string
{
    return currentMode;
}

auto HypixelAPI::SetCurrentMode(const std::string& mode) -> void
{
    currentMode = mode;
}