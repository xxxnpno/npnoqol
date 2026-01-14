#include "NickManager.h"

#include "../../util/api/HypixelAPI/HypixelAPI.h"
#include "../../util/api/MinecraftCode/MinecraftCode.h"

hypixel::NickManager::NickManager()
    : HypixelStatsModule{ true, HypixelGamemode::Gamemode::ALL }
{

}

hypixel::NickManager::~NickManager() = default;

auto hypixel::NickManager::Update()
{
    for (const auto& [k, v] : HypixelAPI::GetNickList())
    {
        if (!v.warned)
        {
            v.warned = true;
            this->Warn(k);
        }
    }
}

auto hypixel::NickManager::Warn(const std::string& name) -> void
{
    mc->GetThePlayer()->AddChatMessage(std::make_unique<IChatComponent>(std::format("{}{} is nicked!") MinecraftCode::codeToString.at(MinecraftCode::Code::RED), name));
}