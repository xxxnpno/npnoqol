#include "ModuleManager.h"

#include "../hypixel/HypixelStatsModule/HypixelStatsModule.h"
#include "../util/api/HypixelAPI/HypixelAPI.h"

#include "../hypixel/all/AutoGG/AutoGG.h"
#include "../hypixel/all/CurrentGamemode/CurrentGamemode.h"
#include "../hypixel/all/NickManager/NickManager.h"

#include "../hypixel/gamemode/BlitzSurvivalGames/BlitzSurvivalGames.h"

ModuleManager::ModuleManager()
{	
	this->RegisterModule<hypixel::AutoGG>();
	this->RegisterModule<hypixel::CurrentGamemode>();
	this->RegisterModule<hypixel::NickManager>();

	this->RegisterModule<hypixel::BlitzSurvivalGames>();

	chat = std::make_unique<Chat>();
}

ModuleManager::~ModuleManager() = default;

void ModuleManager::Update() const
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
void ModuleManager::RegisterModule()
{
	modules.emplace_back(std::make_unique<M>());
}