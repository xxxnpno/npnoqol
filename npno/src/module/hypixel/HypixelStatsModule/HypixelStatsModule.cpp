#include "HypixelStatsModule.h"

#include "../../util/api/HypixelAPI/HypixelAPI.h"
#include "../../util/MinecraftCode/MinecraftCode.h"

#include <algorithm>
#include <unordered_set>
#include <format>

hypixel::HypixelStatsModule::HypixelStatsModule(const bool enable, const HypixelGamemode::Gamemode gamemode, const std::string& autoGGLine)
    : Module{ enable }
    , gamemode{ gamemode }
{
    if (!autoGGLine.empty())
    {
        HypixelAPI::AddAutoGGLine(autoGGLine);
    }
}

hypixel::HypixelStatsModule::~HypixelStatsModule() = default;

auto hypixel::HypixelStatsModule::SanityCheck() const -> bool
{
    return
        mc->GetThePlayer()->GetInstance() and
        mc->GetTheWorld()->GetInstance() and
        mc->GetIngameGUI()->GetInstance() and
        mc->GetTheWorld()->GetScoreboard()->GetInstance() and
        mc->GetThePlayer()->GetSendQueue()->GetInstance();
}

auto hypixel::HypixelStatsModule::GetGamemode() const -> HypixelGamemode::Gamemode
{
    return this->gamemode;
}

auto hypixel::HypixelStatsModule::ClearCache() -> void
{
    this->playerCache.clear();
    this->teamManager.clear();
}

auto hypixel::HypixelStatsModule::UpdateTabList() -> void
{
    const std::unique_ptr<EntityPlayerSP>& thePlayer{ mc->GetThePlayer() };
    const std::unique_ptr<WorldClient>& theWorld{ mc->GetTheWorld() };
    const std::unique_ptr<GuiIngame>& ingameGUI{ mc->GetIngameGUI() };

    const std::vector<std::unique_ptr<NetworkPlayerInfo>>& playerInfoMap{ thePlayer->GetSendQueue()->GetPlayerInfoMap() };

    for (Size i{ 0 }; i < playerInfoMap.size(); ++i)
    {
        const std::unique_ptr<EntityPlayer>& playerEntity{ theWorld->GetPlayerEntityByName(playerInfoMap[i]->GetGameProfile()->GetName()) };

        if (playerEntity->GetInstance())
        {
            if (this->IsBot(playerEntity))
            {
                continue;
            }

            playerInfoMap[i]->SetDisplayName(std::make_unique<ChatComponentText>(this->FormatTabName(playerEntity)));
        }
    }
}

auto hypixel::HypixelStatsModule::UpdateNameTags() -> void
{
    const std::unique_ptr<WorldClient> theWorld{ mc->GetTheWorld() };
    const std::unique_ptr<Scoreboard> scoreboard{ theWorld->GetScoreboard() };

    scoreboard->SetObjectiveInDisplaySlot(Scoreboard::DisplaySlot::BELOW_NAME, nullptr);

    if (this->teamManager.size() < theWorld->GetPlayerEntities().size())
    {
        for (Size i{ this->teamManager.size() }; i < theWorld->GetPlayerEntities().size(); ++i)
        {
            Team team{};
            team.playerName = theWorld->GetPlayerEntities()[i]->GetName();
            team.hypixelTeam = scoreboard->GetTeam(team.playerName)->GetTeamName();
            team.npnoTeam = std::format("npno_{}", i);

            this->teamManager.push_back(std::move(team));
        }
    }

    for (const std::unique_ptr<ScorePlayerTeam>& scoreTeam : scoreboard->GetTeams())
    {
        const std::string hypixelTeamName = scoreTeam->GetTeamName();

        for (const std::string& playerName : scoreTeam->GetMembershipCollection())
        {
            if (!theWorld->GetPlayerEntityByName(playerName)->GetInstance())
            {
                continue;
            }

            if (!hypixelTeamName.starts_with("npno_"))
            {
                if (!this->GetTeamEntry(playerName))
                {
                    Team team{};
                    team.playerName = playerName;
                    team.hypixelTeam = hypixelTeamName;
                    team.npnoTeam = std::format("npno_{}", this->teamManager.size());
                    this->teamManager.push_back(std::move(team));
                }
                else
                {
                    this->GetTeamEntry(playerName)->hypixelTeam = hypixelTeamName;
                }
            }
        }
    }

    for (const std::unique_ptr<EntityPlayer>& player : theWorld->GetPlayerEntities())
    {
        const std::string& playerName = player->GetName();

        if (this->IsBot(player))
        {
            continue;
        }

        std::unique_ptr<ScorePlayerTeam> team{ scoreboard->GetTeam(this->GetTeamFromTeamManager(playerName).npnoTeam) };

        if (!team->GetInstance())
        {
            team = scoreboard->CreateTeam(this->GetTeamFromTeamManager(playerName).npnoTeam);
            if (!team->GetInstance()) continue;
        }

        if (!scoreboard->GetTeam(playerName)->GetTeamName().starts_with("npno_"))
        {
            scoreboard->RemovePlayerFromTeam(playerName, scoreboard->GetTeam(playerName));

            const bool unused = scoreboard->AddPlayerToTeam(playerName, this->GetTeamFromTeamManager(playerName).npnoTeam);
        }

        const std::pair<std::string, std::string> nametag = this->FormatNametag(player);
        team->SetNamePrefix(JavaUtil::FixString(nametag.first));
        team->SetNameSuffix(JavaUtil::FixString(nametag.second));
    }

    this->OrginizeTeams();
}

auto hypixel::HypixelStatsModule::IsBot(const std::unique_ptr<EntityPlayer>& player) -> bool
{
    return player->GetUniqueID()->Version() == 2;
}

auto hypixel::HypixelStatsModule::LoadMissingPlayers() -> void
{
    std::vector<std::string> playerNames;

    for (const std::unique_ptr<EntityPlayer>& player : mc->GetTheWorld()->GetPlayerEntities())
    {
        if (this->IsBot(player))
        {
            continue;
        }

        const std::string& playerName = player->GetName();
        auto it = this->playerCache.find(playerName);

        if (it == this->playerCache.end() or it->second.error)
        {
            if (it != this->playerCache.end() and it->second.error)
            {
                this->playerCache.erase(it);
            }
            playerNames.push_back(playerName);
        }
    }

    if (!playerNames.empty())
    {
        this->LoadPlayersData(playerNames);
    }
}

auto hypixel::HypixelStatsModule::GetPlayerData(const std::string& playerName) -> Player
{
    if (auto it = this->playerCache.find(playerName); it != this->playerCache.end())
    {
        return it->second;
    }

    Player playerData{};
    playerData.error = true;

    return playerData;
}

auto hypixel::HypixelStatsModule::GetHpColor(const float hp) const -> std::string
{
    if (hp >= 20.0f) return MinecraftCode::codeToString.at(MinecraftCode::Code::DARK_GREEN);
    if (hp >= 10.0f) return MinecraftCode::codeToString.at(MinecraftCode::Code::GREEN);
    if (hp >= 5.0f)  return MinecraftCode::codeToString.at(MinecraftCode::Code::YELLOW);
    return MinecraftCode::codeToString.at(MinecraftCode::Code::RED);
}

auto hypixel::HypixelStatsModule::GetTeamFromTeamManager(const std::string& playerName) const -> Team
{
    auto it = std::find_if(this->teamManager.begin(), this->teamManager.end(),
            [&](const auto& entry) 
            {
                return entry.playerName == playerName;
            }
        );

        if (it != this->teamManager.end()) 
        {
            return *it;
        } 
        else 
        {
            return Team{};
        }
}

auto hypixel::HypixelStatsModule::GetTeamEntry(const std::string& playerName) -> Team*
{
    auto it = std::find_if(teamManager.begin(), teamManager.end(),
        [&](const Team& team) 
        { 
            return team.playerName == playerName;
        });

    return it != teamManager.end() ? &(*it) : nullptr;
}

auto hypixel::HypixelStatsModule::SentByServer(const std::string& line) const -> bool
{
    I32 count = 0;
    for (const char c : line)
    {
        if (c == ':') 
        {
            count++;
        }
    }
    return count;
}

auto hypixel::HypixelStatsModule::OrginizeTeams() -> void
{
    this->sortedTeams.clear();

    for (const auto& player : teamManager)
    {
        sortedTeams[player.hypixelTeam].push_back(player);
    }

    for (auto& [teamName, players] : sortedTeams)
    {
        std::sort(players.begin(), players.end(), 
            [](const Team& a, const Team& b) 
            {
                return a.playerName < b.playerName;
            });
    }
}