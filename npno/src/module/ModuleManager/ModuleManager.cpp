#include "ModuleManager.h"

#include "../hypixel/BlitzSurvivalGames/BlitzSurvivalGames.h"

ModuleManager::ModuleManager()
{
	this->RegisterModule<hypixel::BlitzSurvivalGames>();
}

ModuleManager::~ModuleManager() = default;

void ModuleManager::Update() const
{
	for (const std::unique_ptr<Module>& module : this->modules)
	{
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