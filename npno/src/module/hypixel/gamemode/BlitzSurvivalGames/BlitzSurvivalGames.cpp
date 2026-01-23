#include "BlitzSurvivalGames.h"

hypixel::BlitzSurvivalGames::BlitzSurvivalGames()
    : HypixelStatsModule{
        false,
        HypixelGamemode::Gamemode::BLITZSURVIVALGAMES,
        "Winner - ",
        'IDK FOR NOW' }
{
    this->mode = Mode::LOBBY;
}

hypixel::BlitzSurvivalGames::~BlitzSurvivalGames() = default;

auto hypixel::BlitzSurvivalGames::Update() -> void
{
    this->HandleMode();

    if (this->mode == Mode::LOBBY) return;

    this->IsEveryoneLoaded()
    {
        this->UpdateTabList();
    }
    
    this->UpdateNameTags();
}

auto hypixel::BlitzSurvivalGames::LoadPlayersData(const std::vector<std::string>& playerNames) -> void
{
    const std::vector<nlohmann::json>& responses = Network::GetBatchPlayerStats(playerNames);

    for (Size i = 0; i < playerNames.size(); ++i)
    {
        const std::string& playerName = playerNames[i];
        const nlohmann::json& response = responses[i];
        Player playerData{};

        try
        {
            if (HypixelAPI::IsNicked(response))
            {
                playerData.prefix = std::format("{}[NICK]{}",
                    MinecraftCode::codeToString.at(MinecraftCode::Code::RED),
                    MinecraftCode::codeToString.at(MinecraftCode::Code::WHITE)
                );
                playerData.isNick = true;

                HypixelAPI::AddNickPlayer(playerName);

                this->playerCache[playerName] = playerData;
                continue;
            }

            playerData.rank = HypixelRank::GetRankPrefix(response);

            const auto& hg = response["player"]["stats"]["HungerGames"];

            const I32 wins = hg.value("wins", 0) + hg.value("wins_teams", 0);
            const I32 kills = hg.value("kills", 0);
            const I32 deaths = hg.value("deaths", 0);

            playerData.prefix = std::to_string(wins);
            playerData.suffix = std::format("{:.1f}", static_cast<float>(kills) / max(1, deaths));

            this->playerCache[playerName] = playerData;
        }
        catch (...)
        {
            playerData.error = true;
            this->playerCache[playerName] = playerData;
        }
    }
}

auto hypixel::BlitzSurvivalGames::HandleMode() -> void
{
    const std::string currentMode = HypixelAPI::GetCurrentMode();

    if ((this->mode == Mode::SOLO and currentMode == Mode::TEAMS) or (this->mode == Mode::TEAMS and currentMode == Mode::TEAMS))
    {
        this->ClearCache();
    }

    if (currentMode == "solo_normal")
    {
        this->mode = Mode::SOLO;
    }
    else if (currentMode == "teams_normal")
    {
        this->mode = Mode::TEAMS;
    }
    else
    {
        this->mode = Mode::LOBBY;
    }
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

    const std::string rankSection = playerData.rank.empty() ? "" : (playerData.rank + " ");
    return std::format(" {}[{}] {}{} {}{:.1f} {}; {}{}",
        this->GetWinsColor(playerData.prefix),
        playerData.prefix,
        rankSection,
        player->GetName(),
        this->GetHpColor(health),
        health,
        MinecraftCode::codeToString.at(MinecraftCode::Code::AQUA),
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