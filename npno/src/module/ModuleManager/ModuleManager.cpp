#include "ModuleManager.h"

#include "../hypixel/HypixelStatsModule/HypixelStatsModule.h"
#include "../../util/api/HypixelAPI/HypixelAPI.h"

#include "../hypixel/all/AutoGG/AutoGG.h"
#include "../hypixel/all/GamemodeManager/GamemodeManager.h"
#include "../hypixel/all/NickManager/NickManager.h"
#include "../hypixel/all/ScoreboardManager/ScoreboardManager.h"

#include "../hypixel/gamemode/BlitzSurvivalGames/BlitzSurvivalGames.h"

ModuleManager::ModuleManager()
{	
	this->RegisterModule<hypixel::AutoGG>();
	this->RegisterModule<hypixel::GamemodeManager>();
	this->RegisterModule<hypixel::NickManager>();
	this->RegisterModule<hypixel::ScoreboardManager>();

	this->RegisterModule<hypixel::BlitzSurvivalGames>();

	chat = std::make_unique<Chat>();
}

ModuleManager::~ModuleManager() = default;

auto ModuleManager::Update() const -> void
{
	chat->Update();
	
	for (const std::unique_ptr<Module>& module : this->modules)
	{
		if (auto* hypixelModule = dynamic_cast<hypixel::HypixelStatsModule*>(module.get()))
		{
			if (hypixelModule->GetGamemode() != HypixelGamemode::Gamemode::ALL)
			{
				hypixelModule->SetEnable(hypixelModule->GetGamemode() == HypixelAPI::GetCurrentGamemode());
			}

			if (!hypixelModule->IsEnable())
			{
				hypixelModule->ClearCache();
			}
		}
		
		if (module->IsEnable() && module->SanityCheck())
		{
			module->Update();
		}
	}
}

template<typename M>
auto ModuleManager::RegisterModule() -> void
{
	modules.emplace_back(std::make_unique<M>());
}