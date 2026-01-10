#include "BlitzSurvivalGames.h"

hypixel::BlitzSurvivalGames::BlitzSurvivalGames()
    : Module{ true }
{

}

hypixel::BlitzSurvivalGames::~BlitzSurvivalGames() = default;

auto hypixel::BlitzSurvivalGames::SanityCheck() const -> bool
{
    if (!this->mc->GetTheWorld() or !this->mc->GetTheWorld()->GetInstance())
    {
        return false;
    }

    if (!this->mc->GetThePlayer() or !this->mc->GetThePlayer()->GetInstance())
    {
        return false;
    }

    if (!this->mc->GetIngameGUI() or !this->mc->GetIngameGUI()->GetInstance())
    {
        return false;
    }

    const std::unique_ptr<EntityPlayerSP> thePlayer{ this->mc->GetThePlayer() };
    if (!thePlayer->GetSendQueue() or !thePlayer->GetSendQueue()->GetInstance())
    {
        return false;
    }

    const std::unique_ptr<WorldClient> theWorld{ this->mc->GetTheWorld() };
    if (!theWorld->GetScoreboard() or !theWorld->GetScoreboard()->GetInstance())
    {
        return false;
    }

    const std::unique_ptr<NetHandlerPlayClient> sendQueue = thePlayer->GetSendQueue();

    const std::vector<std::unique_ptr<NetworkPlayerInfo>> playerInfoMap{ sendQueue->GetPlayerInfoMap() };
    for (std::size_t i{ 0 }; i < playerInfoMap.size(); ++i)
    {
        if (!playerInfoMap[i] or !playerInfoMap[i]->GetInstance())
        {
            return false;
        }

        if (!playerInfoMap[i]->GetGameProfile() or !playerInfoMap[i]->GetGameProfile()->GetInstance())
        {
            return false;
        }
    }

    const std::vector<std::unique_ptr<EntityPlayer>>& playerEntities = theWorld->GetPlayerEntities();
    for (const std::unique_ptr<EntityPlayer>& player : playerEntities)
    {
        if (!player or !player->GetInstance())
        {
            return false;
        }
    }

    const std::vector<std::unique_ptr<ScorePlayerTeam>>& teams = theWorld->GetScoreboard()->GetTeams();
    for (const std::unique_ptr<ScorePlayerTeam>& team : teams)
    {
        if (!team or !team->GetInstance())
        {
            return false;
        }
    }

    return true;
}

auto hypixel::BlitzSurvivalGames::Update() -> void
{
    this->UpdateTabList();
    this->UpdateNameTags();
}

auto hypixel::BlitzSurvivalGames::UpdateTabList() const -> void
{
    const std::unique_ptr<EntityPlayerSP> thePlayer{ this->mc->GetThePlayer() };
    const std::unique_ptr<WorldClient> theWorld{ this->mc->GetTheWorld() };
    const std::unique_ptr<GuiIngame> ingameGUI{ this->mc->GetIngameGUI() };

    const std::unique_ptr<NetHandlerPlayClient> sendQueue = thePlayer->GetSendQueue();

    const std::vector<std::unique_ptr<NetworkPlayerInfo>> playerInfoMap{ sendQueue->GetPlayerInfoMap() };

    for (std::size_t i{ 0 }; i < playerInfoMap.size(); ++i)
    {
        const std::unique_ptr<EntityPlayer> player{ theWorld->GetPlayerEntityByName(playerInfoMap[i]->GetGameProfile()->GetName()) };

        playerInfoMap[i]->SetDisplayName(std::make_unique<ChatComponentText>(this->FormatTabName(player)));
    }
}

auto hypixel::BlitzSurvivalGames::UpdateNameTags() const -> void
{
    const std::unique_ptr<WorldClient> theWorld{ this->mc->GetTheWorld() };

    std::vector<std::string> playerNames;
    for (const std::unique_ptr<EntityPlayer>& player : theWorld->GetPlayerEntities())
    {
        playerNames.push_back(player->GetName());
    }

    for (const std::unique_ptr<ScorePlayerTeam>& team : theWorld->GetScoreboard()->GetTeams())
    {
        bool hasAtLeastOnePlayer = false;

        for (const std::string& member : team->GetMembershipCollection())
        {
            if (std::find(playerNames.begin(), playerNames.end(), member) != playerNames.end())
            {
                hasAtLeastOnePlayer = true;
                break;
            }
        }

        if (hasAtLeastOnePlayer)
        {
            for (const std::string& member : team->GetMembershipCollection())
            {
                const std::pair<std::string, std::string> nametag = this->FormatNametag(theWorld->GetPlayerEntityByName(member));
                team->SetNamePrefix(nametag.first);
                team->SetNameSuffix(nametag.second);
            }
        }
    }
}

auto hypixel::BlitzSurvivalGames::FormatTabName(const std::unique_ptr<EntityPlayer>& player) const -> std::string
{
    return std::format(" {} {}{:.1f}", player->GetName(), this->GetHpColor(player->GetHealth()), player->GetHealth());
}

auto hypixel::BlitzSurvivalGames::FormatNametag(const std::unique_ptr<EntityPlayer>& player) const -> std::pair<std::string, std::string>
{
	std::pair<std::string, std::string> nametag;

    nametag.first = std::format("");
	nametag.second = std::format(" {}{:.1f}", this->GetHpColor(player->GetHealth()), player->GetHealth());

	JavaUtil::FixString(nametag.first);
	JavaUtil::FixString(nametag.second);

    return nametag;
}

auto hypixel::BlitzSurvivalGames::GetHpColor(const float hp) const -> std::string_view
{
    if (hp >= 20.0f) return MinecraftCode::codeToString[MinecraftCode::Code::DARK_GREEN];
    if (hp >= 10.0f) return MinecraftCode::codeToString[MinecraftCode::Code::GREEN];
    if (hp >= 5.0f)  return MinecraftCode::codeToString[MinecraftCode::Code::YELLOW];

    return MinecraftCode::codeToString[MinecraftCode::Code::RED];
}