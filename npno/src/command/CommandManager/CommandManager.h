#pragma once

#include "../Command/Command.h"

#include <vector>
#include <memory>

class CommandManager final
{
public:
	CommandManager();

	~CommandManager();

	auto Update() const -> void;

private:
	template<typename C>
	auto RegisterCommand() -> void;

	std::vector<std::unique_ptr<Command>> commands;
};