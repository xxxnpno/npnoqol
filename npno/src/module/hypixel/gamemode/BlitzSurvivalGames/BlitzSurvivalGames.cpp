#include "BlitzSurvivalGames.h"

#include "../../../util/HypixelRank/HypixelRank.h"
#include "../../../util/api/HypixelAPI/HypixelAPI.h"
#include "../../../util/MinecraftCode/MinecraftCode.h"

#include <cmath>
#include <algorithm>
#include <string>
#include <format>

hypixel::BlitzSurvivalGames::BlitzSurvivalGames()
    : HypixelStatsModule{
        false,
        HypixelGamemode::Gamemode::BLITZSURVIVALGAMES,
        "Winner - " }
{

}

hypixel::BlitzSurvivalGames::~BlitzSurvivalGames() = default;

auto hypixel::BlitzSurvivalGames::Update() -> void
{
    if (HypixelAPI::GetCurrentMode() == "unknown")
    {
        return;
    }

    const std::unique_ptr<WorldClient>& theWorld{ mc->GetTheWorld() };
    std::vector<std::string> playerNames;

    for (const std::unique_ptr<EntityPlayer>& player : theWorld->GetPlayerEntities())
    {
        if (!this->IsBot(player) and playerCache.find(player->GetName()) == playerCache.end())
        {
            playerNames.push_back(player->GetName());
        }
    }

    if (!playerNames.empty())
    {
        this->LoadPlayersData(playerNames);
    }

    bool allLoaded = true;
    for (const std::unique_ptr<EntityPlayer>& player : theWorld->GetPlayerEntities())
    {
        if (!this->IsBot(player) and playerCache.find(player->GetName()) == playerCache.end())
        {
            allLoaded = false;
            break;
        }
    }

    if (allLoaded)
    {
        this->UpdateTabList();
    }

    this->UpdateNameTags();
}

auto hypixel::BlitzSurvivalGames::LoadPlayersData(const std::vector<std::string>& playerNames) -> void
{
    std::vector<std::string> endpoints;
    endpoints.reserve(playerNames.size());

    for (const auto& name : playerNames)
    {
        endpoints.push_back(std::format("&name={}", name));
    }

    std::vector<std::string> responses = Network::GetBatch(endpoints);

    for (size_t i = 0; i < playerNames.size(); ++i)
    {
        const std::string& playerName = playerNames[i];
        const std::string& response = responses[i];

        Player playerData{};
        playerData.prefix = "";
        playerData.rank = "";
        playerData.suffix = "";
        playerData.isNick = false;
        playerData.error = false;

        if (response.empty())
        {
            playerData.error = true;
            playerCache[playerName] = playerData;
            continue;
        }

        try
        {
            const auto jsonResponse = nlohmann::json::parse(response);

            if (HypixelAPI::IsNicked(jsonResponse))
            {
                playerData.prefix = std::format("{}[NICK]{}",
                    MinecraftCode::codeToString.at(MinecraftCode::Code::RED),
                    MinecraftCode::codeToString.at(MinecraftCode::Code::WHITE)
                );
                playerData.isNick = true;

                HypixelAPI::AddNickPlayer(playerName);

                playerCache[playerName] = playerData;
                continue;
            }

            playerData.rank = HypixelRank::GetRankPrefix(jsonResponse);

            const auto& hg = jsonResponse["player"]["stats"]["HungerGames"];

            const I32 wins = hg.value("wins", 0) + hg.value("wins_teams", 0);
            const I32 kills = hg.value("kills", 0);
            const I32 deaths = hg.value("deaths", 0);

            playerData.prefix = std::to_string(wins);
            playerData.suffix = std::format("{:.1f}", static_cast<float>(kills) / max(1, deaths));

            playerCache[playerName] = playerData;
        }
        catch (...)
        {
            playerData.error = true;
            playerCache[playerName] = playerData;
        }
    }
}

auto hypixel::BlitzSurvivalGames::GetPlayerData(const std::string& playerName) -> Player
{
    if (auto it = playerCache.find(playerName); it != playerCache.end())
    {
        return it->second;
    }

    Player playerData{};
    playerData.error = true;
    return playerData;
}

auto hypixel::BlitzSurvivalGames::FormatTabName(const std::unique_ptr<EntityPlayer>& player) -> std::string
{
    const Player playerData = this->GetPlayerData(player->GetName());
    const float health = player->GetHealth() + player->GetAbsorptionAmount();

    if (playerData.error)
    {
        return std::format(" {}? {}{} {}{:.1f}",
            MinecraftCode::codeToString.at(MinecraftCode::Code::DARK_RED),
            MinecraftCode::codeToString.at(MinecraftCode::Code::AQUA),
            player->GetName(),
            this->GetHpColor(health),
            health
        );
    }

    if (playerData.isNick)
    {
        return std::format(" {} {} {}{:.1f}",
            playerData.prefix,
            player->GetName(),
            this->GetHpColor(health),
            health
        );
    }
    
	const std::string rank = playerData.rank.empty() ? "" : playerData.rank;
    return std::format(" {}[{}] {} {} {}{:.1f} | {}{}",
        this->GetWinsColor(playerData.prefix),
        playerData.prefix,
        rank,
        player->GetName(),
        this->GetHpColor(health),
        health,
        this->GetKDRColor(playerData.suffix),
        playerData.suffix
    );
}

auto hypixel::BlitzSurvivalGames::FormatNametag(const std::unique_ptr<EntityPlayer>& player) -> std::pair<std::string, std::string>
{
    const float health = player->GetHealth() + player->GetAbsorptionAmount();

    std::pair<std::string, std::string> nametag;

    nametag.first = std::format("{} ",
        MinecraftCode::codeToString.at(MinecraftCode::Code::AQUA)
    );

    nametag.second = std::format(" {}{:.1f} ",
        this->GetHpColor(health),
        health
    );

    JavaUtil::FixString(nametag.first);
    JavaUtil::FixString(nametag.second);

    return nametag;
}

auto hypixel::BlitzSurvivalGames::GetHpColor(const float hp) const -> std::string
{
    if (hp >= 20.0f) return MinecraftCode::codeToString.at(MinecraftCode::Code::DARK_GREEN);
    if (hp >= 10.0f) return MinecraftCode::codeToString.at(MinecraftCode::Code::GREEN);
    if (hp >= 5.0f)  return MinecraftCode::codeToString.at(MinecraftCode::Code::YELLOW);
    return MinecraftCode::codeToString.at(MinecraftCode::Code::RED);
}

auto hypixel::BlitzSurvivalGames::GetWinsColor(const std::string& wins) const -> std::string
{
    if (wins.empty()) return MinecraftCode::codeToString.at(MinecraftCode::Code::GRAY);

    int winsValue = std::stoi(wins);
    if (winsValue >= 2500) return MinecraftCode::codeToString.at(MinecraftCode::Code::DARK_RED);
    if (winsValue >= 1000) return MinecraftCode::codeToString.at(MinecraftCode::Code::RED);
    if (winsValue >= 500)  return MinecraftCode::codeToString.at(MinecraftCode::Code::GOLD);
    if (winsValue >= 200)  return MinecraftCode::codeToString.at(MinecraftCode::Code::DARK_GREEN);
    if (winsValue >= 100)  return MinecraftCode::codeToString.at(MinecraftCode::Code::GREEN);

    return MinecraftCode::codeToString.at(MinecraftCode::Code::GRAY);
}

auto hypixel::BlitzSurvivalGames::GetKDRColor(const std::string& kdr) const -> std::string
{
    if (kdr.empty()) return MinecraftCode::codeToString.at(MinecraftCode::Code::GRAY);

    float kdrValue = std::stof(kdr);
    if (kdrValue >= 10.0f) return MinecraftCode::codeToString.at(MinecraftCode::Code::DARK_RED);
    if (kdrValue >= 5.0f)  return MinecraftCode::codeToString.at(MinecraftCode::Code::RED);
    if (kdrValue >= 3.0f)  return MinecraftCode::codeToString.at(MinecraftCode::Code::GOLD);
    if (kdrValue >= 2.0f)  return MinecraftCode::codeToString.at(MinecraftCode::Code::DARK_GREEN);
    if (kdrValue >= 1.0f)  return MinecraftCode::codeToString.at(MinecraftCode::Code::GREEN);

    return MinecraftCode::codeToString.at(MinecraftCode::Code::GRAY);
}