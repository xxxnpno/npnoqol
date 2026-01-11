#include "BlitzSurvivalGames.h"

#include "../../util/HypixelRank/HypixelRank.h"
#include "../../util/api/HypixelAPI/HypixelAPI.h"
#include "../../util/MinecraftCode/MinecraftCode.h"

#include <cmath>

hypixel::BlitzSurvivalGames::BlitzSurvivalGames()
    : HypixelStatsModule{ true }
{

}

hypixel::BlitzSurvivalGames::~BlitzSurvivalGames() = default;

auto hypixel::BlitzSurvivalGames::Update() -> void
{
    this->UpdateTabList();
    //this->UpdateNameTags();
}

auto hypixel::BlitzSurvivalGames::GetPlayerData(const std::string& playerName) -> Player
{
    auto it = playerCache.find(playerName);
    if (it != playerCache.end())
    {
        return it->second;
    }

    Player playerData{};

    try
    {
        nlohmann::json jsonResponse = HypixelAPI::GetPlayerStats(playerName);
        if (HypixelAPI::IsNicked(jsonResponse))
        {
            playerData.rank = std::format(
                "{}{}{}",
                MinecraftCode::codeToString.at(MinecraftCode::Code::RED),
                "[NICK]",
                MinecraftCode::codeToString.at(MinecraftCode::Code::WHITE)
            );
            playerData.prefix = "";
            playerData.suffix = "";
            playerCache[playerName] = playerData;

            return playerData;
        }

        playerData.rank = HypixelRank::GetRankPrefix(jsonResponse);
        
        const auto& hg = jsonResponse["player"]["stats"]["HungerGames"];

        I32 wins = hg.value("wins", 0) + hg.value("wins_teams", 0);
        I32 kills = hg.value("kills", 0);
        I32 deaths = hg.value("deaths", 0);

        playerData.prefix = std::format("{}", wins);
        playerData.suffix = std::format("{:.1f}", static_cast<float>(kills) / max(1, deaths));

        playerCache[playerName] = playerData;
        return playerData;
    }
    catch (...)
    {
        return playerData;
    }
}

auto hypixel::BlitzSurvivalGames::FormatTabName(const std::unique_ptr<EntityPlayer>& player) -> std::string
{
    const Player playerData = this->GetPlayerData(player->GetName());

    return std::format(" {}{} {} {} {}{:.1f} {}{}",
        this->GetWinsColor(playerData.prefix), playerData.prefix,
        playerData.rank,
        player->GetName(),
        this->GetHpColor(player->GetHealth()), player->GetHealth(),
        this->GetKDRColor(playerData.suffix), playerData.suffix);
}

auto hypixel::BlitzSurvivalGames::FormatNametag(const std::unique_ptr<EntityPlayer>& player) -> std::pair<std::string, std::string>
{
    const Player playerData = this->GetPlayerData(player->GetName());

    std::pair<std::string, std::string> nametag;

    nametag.first = std::format("{}{} {}",
        this->GetWinsColor(playerData.prefix), playerData.prefix,
        playerData.rank);

    nametag.second = std::format(" {}{:.1f} {}{}",
        this->GetHpColor(player->GetHealth()), player->GetHealth(),
        this->GetKDRColor(playerData.suffix), playerData.suffix);

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
    if (wins.empty()) return MinecraftCode::codeToString.at(MinecraftCode::Code::WHITE);

    int winsValue = std::stoi(wins);

    if (winsValue >= 2500) return std::format("{}{}", MinecraftCode::codeToString.at(MinecraftCode::Code::BOLD), MinecraftCode::codeToString.at(MinecraftCode::Code::DARK_RED));
    if (winsValue >= 1000) return MinecraftCode::codeToString.at(MinecraftCode::Code::RED);
    if (winsValue >= 500)  return MinecraftCode::codeToString.at(MinecraftCode::Code::GOLD);
    if (winsValue >= 200)  return MinecraftCode::codeToString.at(MinecraftCode::Code::DARK_GREEN);
    if (winsValue >= 100)  return MinecraftCode::codeToString.at(MinecraftCode::Code::GREEN);
    return MinecraftCode::codeToString.at(MinecraftCode::Code::GRAY);
}

auto hypixel::BlitzSurvivalGames::GetKDRColor(const std::string& kdr) const -> std::string
{
    if (kdr.empty()) return MinecraftCode::codeToString.at(MinecraftCode::Code::WHITE);

    float kdrValue = std::stof(kdr);

    if (kdrValue >= 10.0f) return std::format("{}{}", MinecraftCode::codeToString.at(MinecraftCode::Code::BOLD), MinecraftCode::codeToString.at(MinecraftCode::Code::DARK_RED));
    if (kdrValue >= 5.0f)  return MinecraftCode::codeToString.at(MinecraftCode::Code::RED);
    if (kdrValue >= 3.0f)  return MinecraftCode::codeToString.at(MinecraftCode::Code::GOLD);
    if (kdrValue >= 2.0f)  return MinecraftCode::codeToString.at(MinecraftCode::Code::DARK_GREEN);
    if (kdrValue >= 1.0f)  return MinecraftCode::codeToString.at(MinecraftCode::Code::GREEN);
    return MinecraftCode::codeToString.at(MinecraftCode::Code::GRAY);
}