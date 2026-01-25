#include "CommandManager.h"

#include "../SetHypixelAPIKeyCommand/SetHypixelAPIKeyCommand.h"

CommandManager::CommandManager()
{
	this->RegisterCommand<SetHypixelAPIKeyCommand>();
}

CommandManager::~CommandManager() = default;

auto CommandManager::Update() const -> void
{
	for (const std::unique_ptr<Command>& command : this->commands)
	{
		command->Proc();
	}
}

template<typename C>
auto CommandManager::RegisterCommand() -> void
{
	commands.emplace_back(std::make_unique<C>());
}