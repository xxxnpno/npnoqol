#pragma once

#include "../../Module/Module.h"

#include "../../util/MinecraftCode/MinecraftCode.h"

namespace hypixel
{
	class BlitzSurvivalGames final : public Module
	{
	public:
		BlitzSurvivalGames();

		~BlitzSurvivalGames() override;

		auto Update() -> void override;

	private:
		auto UpdateTabList() const -> void;

		auto FormatName(const std::unique_ptr<EntityPlayer>& player) const -> std::string;

		auto GetHpColor(const float hp) const -> std::string_view;
	};
}