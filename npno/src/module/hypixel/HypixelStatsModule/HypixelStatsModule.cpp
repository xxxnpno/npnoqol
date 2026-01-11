#include "HypixelStatsModule.h"

#include <algorithm>

hypixel::HypixelStatsModule::HypixelStatsModule(const bool enable)
    : Module{ enable }
{

}

hypixel::HypixelStatsModule::~HypixelStatsModule() = default;

auto hypixel::HypixelStatsModule::SanityCheck() const -> bool
{
    return
        this->mc->GetThePlayer() and
        this->mc->GetTheWorld() and
        this->mc->GetIngameGUI() and
        this->mc->GetTheWorld()->GetScoreboard() and
        this->mc->GetThePlayer()->GetSendQueue();
}

auto hypixel::HypixelStatsModule::UpdateTabList() -> void
{
    const std::unique_ptr<EntityPlayerSP> thePlayer{ this->mc->GetThePlayer() };
    const std::unique_ptr<WorldClient> theWorld{ this->mc->GetTheWorld() };
    const std::unique_ptr<GuiIngame> ingameGUI{ this->mc->GetIngameGUI() };

    const std::vector<std::unique_ptr<NetworkPlayerInfo>> playerInfoMap{ thePlayer->GetSendQueue()->GetPlayerInfoMap() };

    for (size_t i{ 0 }; i < playerInfoMap.size(); ++i)
    {
        playerInfoMap[i]->SetDisplayName(std::make_unique<ChatComponentText>(this->FormatTabName(theWorld->GetPlayerEntityByName(playerInfoMap[i]->GetGameProfile()->GetName()))));
    }
}

auto hypixel::HypixelStatsModule::UpdateNameTags() -> void
{
    const std::unique_ptr<WorldClient> theWorld{ this->mc->GetTheWorld() };

    std::vector<std::string> playerNames;
    for (const std::unique_ptr<EntityPlayer>& player : theWorld->GetPlayerEntities())
    {
        playerNames.push_back(player->GetName());
    }

    for (const std::unique_ptr<ScorePlayerTeam>& team : theWorld->GetScoreboard()->GetTeams())
    {
        bool onePlayer = false;

        for (const std::string& member : team->GetMembershipCollection())
        {
            if (std::find(playerNames.begin(), playerNames.end(), member) != playerNames.end())
            {
                onePlayer = true;
                break;
            }
        }

        if (onePlayer)
        {
            for (const std::string& member : team->GetMembershipCollection())
            {
                const std::unique_ptr<EntityPlayer> playerEntity{ theWorld->GetPlayerEntityByName(member) };
                if (playerEntity)
                {
                    const std::pair<std::string, std::string> nametag = this->FormatNametag(playerEntity);
                    team->SetNamePrefix(nametag.first);
                    team->SetNameSuffix(nametag.second);
                }
            }
        }
    }
}