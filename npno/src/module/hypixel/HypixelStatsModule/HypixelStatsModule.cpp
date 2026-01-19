#include "HypixelStatsModule.h"

#include "../../util/api/HypixelAPI/HypixelAPI.h"

#include <algorithm>
#include <unordered_set>

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
    const std::unique_ptr<WorldClient>& theWorld{ this->mc->GetTheWorld() };
    const std::unique_ptr<Scoreboard>& scoreboard{ theWorld->GetScoreboard() };

    static std::vector<std::string> teamPool;

    const Size playerCount = theWorld->GetPlayerEntities().size();
    const Size requiredTeams = min(playerCount, static_cast<Size>(theWorld->GetPlayerEntities().size()));

    if (teamPool.size() < requiredTeams)
    {
        for (Size i = teamPool.size(); i < requiredTeams; ++i)
        {
            teamPool.push_back(std::format("{}", i));
        }
    }

    Size teamIndex = 0;

    for (const std::unique_ptr<EntityPlayer>& player : theWorld->GetPlayerEntities())
    {   
        if (this->IsBot(player))
        {
            continue;
        }
        
        if (teamIndex >= teamPool.size()) break;

        const std::string& playerName = player->GetName();
        const std::string& teamName = teamPool[teamIndex++];

        std::unique_ptr<ScorePlayerTeam> team{ scoreboard->GetTeam(teamName) };

        if (!team->GetInstance())
        {
            team = scoreboard->CreateTeam(teamName);
            if (!team->GetInstance()) continue;
        }

        std::vector<std::string> members = team->GetMembershipCollection();
        for (const std::string& member : members)
        {
            scoreboard->RemovePlayerFromTeam(member, team);
        }

        const bool unused = scoreboard->AddPlayerToTeam(playerName, teamName);

        const std::pair<std::string, std::string> nametag = this->FormatNametag(player);
        team->SetNamePrefix(nametag.first);
        team->SetNameSuffix(nametag.second);
    }
}

auto hypixel::HypixelStatsModule::IsBot(const std::unique_ptr<EntityPlayer>& player) -> bool
{
    return player->GetUniqueID()->Version() == 2;
}