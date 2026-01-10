#include "BlitzSurvivalGames.h"

hypixel::BlitzSurvivalGames::BlitzSurvivalGames()
    : Module{ true }
{

}

hypixel::BlitzSurvivalGames::~BlitzSurvivalGames() = default;

auto hypixel::BlitzSurvivalGames::Update() -> void
{
    this->UpdateTabList();
}

auto hypixel::BlitzSurvivalGames::UpdateTabList() const -> void
{
    const std::unique_ptr<EntityPlayerSP> thePlayer = this->mc->GetThePlayer();
    const std::unique_ptr<WorldClient> theWorld = this->mc->GetTheWorld();
    const std::unique_ptr<GuiIngame> ingameGUI = this->mc->GetIngameGUI();

    const std::vector<std::unique_ptr<NetworkPlayerInfo>> playerInfoMap = thePlayer->GetSendQueue()->GetPlayerInfoMap();

    for (std::size_t i{ 0 }; i < playerInfoMap.size(); ++i)
    {
        const std::string playerName = playerInfoMap[i]->GetGameProfile()->GetName();
        const std::unique_ptr<EntityPlayer> player = theWorld->GetPlayerEntityByName(playerName);

        playerInfoMap[i]->SetDisplayName(std::make_unique<ChatComponentText>(this->FormatName(player)));
    }
}

auto hypixel::BlitzSurvivalGames::FormatName(const std::unique_ptr<EntityPlayer>& player) const -> std::string
{
    return std::format(" {} {}{}", player->GetName(), this->GetHpColor(player->GetHealth()), player->GetHealth());
}

auto hypixel::BlitzSurvivalGames::GetHpColor(const float hp) const -> std::string_view
{
    if (hp >= 20.0f) return MinecraftCode::codeToString[MinecraftCode::Code::DARK_GREEN];
    if (hp >= 10.0f) return MinecraftCode::codeToString[MinecraftCode::Code::GREEN];
    if (hp >= 5.0f)  return MinecraftCode::codeToString[MinecraftCode::Code::YELLOW];

    return MinecraftCode::codeToString[MinecraftCode::Code::RED];
}