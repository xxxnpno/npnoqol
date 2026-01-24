#include "BlitzSurvivalGames.h"

hypixel::BlitzSurvivalGames::BlitzSurvivalGames()
    : HypixelStatsModule{
        false,
        HypixelGamemode::Gamemode::BLITZSURVIVALGAMES,
        "Winner - " }
{
    this->mode = Mode::LOBBY;
}

hypixel::BlitzSurvivalGames::~BlitzSurvivalGames() = default;

auto hypixel::BlitzSurvivalGames::Update() -> void
{
    this->HandleMode();

    if (this->mode == Mode::LOBBY) return;

    this->LoadMissingPlayers();

    this->UpdateTabList();
    this->UpdateNameTags();

	this->AssignTeamNumbers();
	this->AssignTeamColors();
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

            const auto& hg = response.at("player").at("stats").at("HungerGames");

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

    if ((this->mode == Mode::SOLO and currentMode == "teams_normal") or (this->mode == Mode::TEAMS and currentMode == "solo_normal"))   
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
    Player playerData = this->GetPlayerData(player->GetName());
    const float health = player->GetHealth() + player->GetAbsorptionAmount();

    if (this->mode == Mode::TEAMS)
    {
        const std::string& teamName = this->GetTeamFromTeamManager(player->GetName()).hypixelTeam;

        auto it = teamColors.find(teamName);
        if (it != teamColors.end())
        {
            playerData.prefix = it->second + playerData.prefix;
        }
    }

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

    if (this->mode == Mode::TEAMS)
    {
        const std::string& teamName = this->GetTeamFromTeamManager(player->GetName()).hypixelTeam;

        auto it = teamColors.find(teamName);
        if (it != teamColors.end())
        {
            nametag.first = nametag.first + it->second;
        }
    }

    return nametag;
}

auto hypixel::BlitzSurvivalGames::GetWinsColor(const std::string& wins) const -> std::string
{
    if (wins.empty()) return MinecraftCode::codeToString.at(MinecraftCode::Code::GRAY);

    if (!std::all_of(wins.begin(), wins.end(), ::isdigit)) 
    {
        return MinecraftCode::codeToString.at(MinecraftCode::Code::GRAY);
    }

    I32 winsValue = std::stoi(wins);
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

auto hypixel::BlitzSurvivalGames::AssignTeamNumbers() -> void
{
    this->teamNumbers.clear();

    U32 counter = 1;
    for (const auto& [teamName, players] : sortedTeams)
    {
        teamNumbers[teamName] = counter++;
    }
}

auto hypixel::BlitzSurvivalGames::GetTeamIndex(const std::string& playerName) const -> I32
{
    std::string teamName = this->GetTeamFromTeamManager(playerName).hypixelTeam;

    if (teamName.find("§") != std::string::npos)
    {
        return -1;
    }

    auto it = teamNumbers.find(teamName);
    if (it != teamNumbers.end())
    {
        return it->second;
    }

    return -1;
}

auto hypixel::BlitzSurvivalGames::AssignTeamColors() -> void
{
    teamColors.clear();

    const std::vector<MinecraftCode::Code> baseColors = 
    {
        MinecraftCode::Code::BLACK,
        MinecraftCode::Code::DARK_BLUE,
        MinecraftCode::Code::DARK_GREEN,
        MinecraftCode::Code::DARK_AQUA,
        MinecraftCode::Code::DARK_RED,
        MinecraftCode::Code::DARK_PURPLE,
        MinecraftCode::Code::GOLD,
        MinecraftCode::Code::GRAY,
        MinecraftCode::Code::BLUE,
        MinecraftCode::Code::GREEN,
        MinecraftCode::Code::AQUA,
        MinecraftCode::Code::RED,
        MinecraftCode::Code::LIGHT_PURPLE,
        MinecraftCode::Code::YELLOW,
        MinecraftCode::Code::WHITE
    };

    U32 i = 0;
    for (const auto& [teamName, _] : sortedTeams)
    {
        std::string color = MinecraftCode::codeToString.at(baseColors[i % baseColors.size()]);

        if (i >= baseColors.size())
        {
            color = MinecraftCode::codeToString.at(MinecraftCode::Code::BOLD) + color;
        }

        teamColors[teamName] = color;
        ++i;
    }
}