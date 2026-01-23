#include "HypixelStatsModule.h"

#include "../../util/api/HypixelAPI/HypixelAPI.h"
#include "../../util/MinecraftCode/MinecraftCode.h"

#include <algorithm>
#include <unordered_set>
#include <format>

hypixel::HypixelStatsModule::HypixelStatsModule(const bool enable, const HypixelGamemode::Gamemode gamemode, const std::string& autoGGLine, const std::string& gameStartsMessage)
    : Module{ enable }
    , gamemode{ gamemode }
    , gameStartsMessage{ gameStartsMessage }
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

    this->modeState = ModeState::NOTINGAME;
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
    /*
        ehm this method is kinda weird
    */
    
    const std::unique_ptr<WorldClient> theWorld{ mc->GetTheWorld() };
    const std::unique_ptr<Scoreboard> scoreboard{ theWorld->GetScoreboard() };

    const std::vector<std::unique_ptr<EntityPlayer>> playerEntities{ theWorld->GetPlayerEntities() };

    /*
        the goal of this check is to save the team assigned by hypixel for each player in teams mode cause I override it later,
        the problem is that in the pregame lobby hypixel doesn't know the real teams so we'll get garbage values, to fix this
        problem I added an enum  ModeState that clear the teamManager when the game really starts.

        so when we are in the pregame lobby we get trash values for teams but we don't care since we don't use it, however when
        the game starts we clear every trash values to obtain real teams
    */
    if (this->ModeState == ModeState::PREGAME)
    {
        this->teamManager.clear();
        this->ModeState = ModeState::PREGAMEANDRELOADED;
    }

    if (this->teamManager.size() < playerEntities.size())
    {
        for (Size i{ this->teamManager.size() }; i < playerEntities.size(); ++i)
        {
            this->teamManager[i].playerName = playerEntities[i]->GetName();
            this->teamManager[i].hypixelTeam = scoreboard->GetTeam(playerEntities[i]->GetName())->GetTeamName();
            this->teamManager[i].npnoTeam = std::format("npno_{}", i);
        }
    }

    /*
        here is we are in the pregame lobby teamManager stores trash hypixelTeam values
        but if we are ingame its store 
    */

    /*
        in this loop we are going to create npnoTeams if needed and update nametags right after
    */
    for (const std::unique_ptr<EntityPlayer>& player : playerEntities)
    {
        const std::string& playerName = player->GetName();

        if (this->IsBot(playerName))
        {
            continue;
        }

        std::unique_ptr<ScorePlayerTeam> team{ scoreboard->GetTeam(this->GetTeamFromTeamManager(playerName).npnoTeam) };

        /*
            if the npnoTeam already exists it's great but if not we create it
        */
        if (!team->GetInstance())
        {
            team = scoreboard->CreateTeam(this->GetTeamFromTeamManager(playerName).npnoTeam);

            /*
                I don't remember if it can fail but checking to be sure
            */
            if (!team->GetInstance()) continue;
        }

        /*
            now we have a vector that associates each current player to his old hypixel team and his current npno team
        */

        /*
            if a player is still in his old hypixel team we remove him from it and we place him in his associated npno team
            keep in mind that the npno team of a player should not be changed during a game, it'll affect his scoreboard
            position and that might get really annoying
        */
        if (!scoreboard->GetTeam(playerName)->GetTeam().starts_with("npno_"))
        {
            scoreboard->RemovePlayerFromTeam(playerName, scoreboard->GetTeam(playerName)->GetTeam());

            const bool unused = scoreboard->AddPlayerToTeam(playerName, this->GetTeamFromTeamManager(playerName).npnoTeam);
        }

        /*
            we can finally update the nametag
        */
        const std::pair<std::string, std::string> nametag = this->FormatNametag(player);
        team->SetNamePrefix(nametag.first);
        team->SetNameSuffix(nametag.second);
    }
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

auto GetTeamFromTeamManager(const std::string& playerName) const -> Team
{
    auto it = std::find_if(this->teamManager.begin(), this->teamManager.end(),
            [&](const auto& entry) 
            {
                return entry.playerName == playerName;
            }
        );

        if (it != this->teamManager.end()) 
        {
            return it->team;
        } 
        else 
        {
            return Team{};
        }
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

auto hypixel::HypixelStatsModule::HandleGameStart() -> void
{
    for(const std::string& line : Chat::GetNewLines())
    {
        if (line.find(gameStartsMessage) != std::string::npos and this->SentByServer(line))
        {
            this->modeState = ModeState::INGAME;
            return;
        }
	}
}