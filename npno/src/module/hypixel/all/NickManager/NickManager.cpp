#include "NickManager.h"


hypixel::NickManager::NickManager()
    : HypixelStatsModule{ true, HypixelGamemode::Gamemode::ALL }
{

}

hypixel::NickManager::~NickManager() = default;

auto hypixel::NickManager::Update() -> void
{
    for (auto& [name, player] : HypixelAPI::GetNickList())
    {
        if (!player.warned)
        {
            player.warned = true;
            this->Warn(name);
        }
    }
}

auto hypixel::NickManager::Warn(const std::string& name) -> void
{
    mc->GetThePlayer()->AddChatMessage(std::make_unique<ChatComponentText>(std::format("{}{} {}is nicked", MinecraftCode::codeToString.at(MinecraftCode::Code::DARK_AQUA), name, MinecraftCode::codeToString.at(MinecraftCode::Code::AQUA))));
}