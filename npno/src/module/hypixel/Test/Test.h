#pragma once

#include "../../Module/Module.h"

namespace hypixel
{
	class Test final : public Module
	{
	public:
		Test();

		~Test() override;

		auto Update() -> void override;
	};
}