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
    HypixelAPI::AddAutoGGLine(autoGGLine);
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
    const std::unique_ptr<WorldClient>& theWorld{ mc->GetTheWorld() };
    const std::unique_ptr<Scoreboard>& scoreboard{ theWorld->GetScoreboard() };

    for (const std::unique_ptr<EntityPlayer>& player : theWorld->GetPlayerEntities())
    {
        if (this->IsBot(player)) continue;

        const std::string& playerName = player->GetName();
        std::string teamName;

        auto it = playerToScoreboardTeam.find(playerName);
        if (it != playerToScoreboardTeam.end())
        {
            teamName = it->second;
        }
        else
        {
            teamName = std::format("npno_{}", nextTeamNumber++);
            playerToScoreboardTeam[playerName] = teamName;
        }

        std::unique_ptr<ScorePlayerTeam> team{ scoreboard->GetTeam(teamName) };

        if (!team->GetInstance())
        {
            team = scoreboard->CreateTeam(teamName);
            if (!team->GetInstance()) continue;
        }

        std::vector<std::string> members = team->GetMembershipCollection();
        bool alreadyInTeam = false;
        for (const std::string& member : members)
        {
            if (member == playerName)
            {
                alreadyInTeam = true;
            }
            else
            {
                scoreboard->RemovePlayerFromTeam(member, team);
            }
        }

        if (!alreadyInTeam)
        {
            scoreboard->AddPlayerToTeam(playerName, teamName);
        }

        const std::pair<std::string, std::string> nametag = this->FormatNametag(player);
        team->SetNamePrefix(nametag.first);
        team->SetNameSuffix(nametag.second);
    }
}

auto hypixel::HypixelStatsModule::IsBot(const std::unique_ptr<EntityPlayer>& player) -> bool
{
    return player->GetUniqueID()->Version() == 2;
}

auto hypixel::HypixelStatsModule::IsEveryoneLoaded() -> bool
{
    const std::unique_ptr<WorldClient>& theWorld{ mc->GetTheWorld() };
    std::vector<std::string> playerNames;

    for (const std::unique_ptr<EntityPlayer>& player : theWorld->GetPlayerEntities())
    {
        if (this->IsBot(player))
        {
            continue;
        }

        const std::string& playerName = player->GetName();
        auto it = this->playerCache.find(playerName);

        if (it == this->playerCache.end() or it->second.error)
        {
            if (it != this->playerCache.end() && it->second.error)
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

    return false;
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

auto hypixel::HypixelStatsModule::DetectTeams(const std::vector<std::unique_ptr<EntityPlayer>>& players) -> void
{
    if (teamsDetected) return;

    const std::unique_ptr<WorldClient>& theWorld{ mc->GetTheWorld() };
    const std::unique_ptr<Scoreboard>& scoreboard{ theWorld->GetScoreboard() };

    playerToScoreboardTeam.clear();
    nextTeamNumber = 1;

    std::unordered_map<std::string, std::vector<std::string>> cannotAttack;

    for (const std::unique_ptr<EntityPlayer>& attacker : players)
    {
        for (const std::unique_ptr<EntityPlayer>& target : players)
        {
            if (attacker->GetName() == target->GetName()) continue;

            if (!attacker->CanAttackPlayer(target))
            {
                cannotAttack[attacker->GetName()].push_back(target->GetName());
            }
        }
    }

    std::unordered_set<std::string> processed;
    I32 teamNumber{ 1 };

    for (const auto& [player, teammates] : cannotAttack)
    {
        if (processed.contains(player)) continue;

        Team team{};
        team.teamNumber = teamNumber++;
        team.members.push_back(player);
        processed.insert(player);
        playerToTeam[player] = team.teamNumber;

        for (const std::string& teammate : teammates)
        {
            if (processed.contains(teammate)) continue;

            team.members.push_back(teammate);
            processed.insert(teammate);
            playerToTeam[teammate] = team.teamNumber;
        }

        teams.push_back(team);
    }

    for (const std::unique_ptr<EntityPlayer>& player : players)
    {
        if (processed.contains(player->GetName())) continue;

        Team soloTeam;
        soloTeam.teamNumber = teamNumber++;
        soloTeam.members.push_back(player->GetName());
        playerToTeam[player->GetName()] = soloTeam.teamNumber;
        teams.push_back(soloTeam);
    }

    teamsDetected = true;
}

bool hypixel::HypixelStatsModule::CheckGameStart(const std::vector<std::unique_ptr<EntityPlayer>>& players)
{
    const std::unique_ptr<EntityPlayerSP>& thePlayer = mc->GetThePlayer();

    if (players.size() < 2)
    {
        return false;
    }

    std::vector<const std::unique_ptr<EntityPlayer>*> enemies;

    for (const std::unique_ptr<EntityPlayer>& player : players)
    {
        if (player->GetName() == thePlayer->GetName())
        {
            continue;
        }

        enemies.push_back(&player);

        if (enemies.size() == 2)
        {
            break;
        }
    }

    if (enemies.size() < 2)
    {
        return false;
    }

    for (const std::unique_ptr<EntityPlayer>* enemy : enemies)
    {
        if (thePlayer->CanAttackPlayer(*enemy))
        {
            return true;
        }
    }

    return false;
}

auto hypixel::HypixelStatsModule::GetPlayerTeamNumber(const std::string& playerName) const -> I32
{
    auto it = playerToTeam.find(playerName);
    return (it != playerToTeam.end()) ? it->second : 0;
}

auto hypixel::HypixelStatsModule::ResetTeams() -> void
{
    teams.clear();
    playerToTeam.clear();
    teamsDetected = false;
    playerToScoreboardTeam.clear();
    nextTeamNumber = 1;
}

auto hypixel::HypixelStatsModule::GetTeamPrefix(const std::string& playerName) const -> std::string
{
    if (gameState != GameState::INGAME) return "";
    
    I32 teamNum = this->GetPlayerTeamNumber(playerName);
    if (teamNum == 0) return "";

    return std::format("[{}] ", teamNum);
}