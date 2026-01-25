#pragma once

#include "../Module/Module.h"

#include "../../util/Chat/Chat.h"

#include <vector>
#include <memory>

class ModuleManager final
{
public:
	ModuleManager();

	~ModuleManager();

	auto Update() const -> void;

private:
	template<typename M>
	auto RegisterModule() -> void;

	std::vector<std::unique_ptr<Module>> modules;

	std::unique_ptr<Chat> chat;
};