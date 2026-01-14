#include "ModuleManager.h"

#include "../hypixel/HypixelStatsModule/HypixelStatsModule.h"
#include "../api/HypixelAPI/HypixelAPI.h"

#include "../cheat/CameraNoClip/CameraNoClip.h"

#include "../hypixel/all/AutoGG/AutoGG.h"
#include "../hypixel/all/CurrentGamemode/CurrentGamemode.h"
#include "../hypixel/all/NickManager/NickManager.h"

#include "../hypixel/gamemode/BlitzSurvivalGames/BlitzSurvivalGames.h"

ModuleManager::ModuleManager()
{
	this->RegisterModule<cheat::CameraNoClip>();
	
	this->RegisterModule<hypixel::AutoGG>();
	this->RegisterModule<hypixel::CurrentGamemode>();
	this->RegisterModule<hypixel::NickManager>();

	this->RegisterModule<hypixel::BlitzSurvivalGames>();
}

ModuleManager::~ModuleManager() = default;

void ModuleManager::Update() const
{
	for (const std::unique_ptr<Module>& module : this->modules)
	{
		if (auto* hypixelModule = dynamic_cast<HypixelStatsModule*>(module.get()))
		{
			if (hypixelModule->GetGamemode() != HypixelGamemode::Gamemode::ALL)
			{
				hypixelModule->SetEnable(hypixelModule->GetGamemode() == HypixelAPI::GetCurrentGamemode())
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