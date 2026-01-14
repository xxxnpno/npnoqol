#pragma once

#include "../../Config/Config.h"
#include "../../HypixelRank/HypixelRank.h"

#include <string>

#include <nlohmann/json.hpp>

class HypixelAPI final
{
public:
	static auto CheckKey() -> bool;

	static auto IsNicked(const nlohmann::json& json) -> bool;

	static auto GetPlayerStats(const std::string& playerName) -> nlohmann::json;
private:
	inline static std::string url{ "https://api.hypixel.net" };

	inline static std::string apiKey{ Config::GetHypixelAPIKey()};
};