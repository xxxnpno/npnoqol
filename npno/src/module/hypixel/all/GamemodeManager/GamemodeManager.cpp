#include "GamemodeManager.h"

hypixel::GamemodeManager::GamemodeManager()
    : HypixelStatsModule{ true, HypixelGamemode::Gamemode::ALL }
{
    
}

hypixel::GamemodeManager::~GamemodeManager() = default;

auto hypixel::GamemodeManager::Update() -> void
{
    for (const std::string& line : Chat::GetNewLines())
    {
        std::string jsonPart = ExtractJson(line);
        if (jsonPart.empty()) continue;

        if (nlohmann::json::accept(jsonPart))
        {
            const auto json = nlohmann::json::parse(jsonPart);

            if (json.contains("gametype"))
            {
                HypixelGamemode::Gamemode gamemode = HypixelGamemode::stringToGamemode.at(json["gametype"].get<std::string>());
                HypixelAPI::SetCurrentGamemode(gamemode);
            }

            if (json.contains("mode"))
            {
				HypixelAPI::SetCurrentMode(json["mode"].get<std::string>());
            }
            else
            {
                HypixelAPI::SetCurrentMode("unknown");
            }
        }
    }
}

auto hypixel::GamemodeManager::ExtractJson(const std::string& line) -> std::string
{
    auto pos = line.find('{');
    if (pos != std::string::npos) 
    {
        return line.substr(pos);
    }
    return "";
}