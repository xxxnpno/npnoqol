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

auto AddNickPlayer(const std::string& playerName) -> void
{
    Nick player;
    player.warned = false;

    nickList.add({playerName, player});
}

auto GetNickList() const -> std::map<std::string, Nick>
{
    return nickList;
}

auto HypixelAPI::AddAutoGGLine(const std::string& line) -> void
{
    autoGGLines.push_back(line);
}

auto HypixelAPI::GetAutoGGLines() const -> std::vector<std::string>
{
    return autoGGLines;
}

auto HypixelAPI::GetCurrentGamemode() const -> HypixelGamemode::Gamemode
{
    return currentGamemode;
}

auto HypixelAPI::SetCurrentGamemode(const HypixelGamemode::Gamemode gamemode) -> void
{
    currentGamemode = gamemode;
}