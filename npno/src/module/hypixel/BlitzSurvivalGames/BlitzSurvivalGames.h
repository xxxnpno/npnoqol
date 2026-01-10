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

		auto SanityCheck() const -> bool override;

		auto Update() -> void override;

	private:
		auto UpdateTabList() const -> void;
		auto UpdateNameTags() const -> void;

		auto FormatTabName(const std::unique_ptr<EntityPlayer>& player) const -> std::string;

		auto FormatNametag(const std::unique_ptr<EntityPlayer>& player) const -> std::pair<std::string, std::string>;

		auto GetHpColor(const float hp) const -> std::string_view;
	};
}