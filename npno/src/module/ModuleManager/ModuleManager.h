#pragma once

#include "../Module/Module.h"

#include <vector>
#include <memory>

class ModuleManager final
{
public:
	ModuleManager();

	~ModuleManager();

	void Update() const;

private:
	template<typename M>
	void RegisterModule();

	std::vector<std::unique_ptr<Module>> modules;
};