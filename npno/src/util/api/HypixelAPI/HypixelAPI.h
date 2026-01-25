#pragma once

#include "../../Network/Network.h"
#include "../../Config/Config.h"
#include "../../HypixelRank/HypixelRank.h"
#include "../../HypixelGamemode/HypixelGamemode.h"

#include <string>
#include <vector>
#include <map>

#include <nlohmann/json.hpp>

class HypixelAPI final
{
public:
	struct Nick
	{
		bool warned;
	};
	
	static auto CheckKey() -> bool;

	static auto IsNicked(const nlohmann::json& json) -> bool;

	static auto GetPlayerStats(const std::string& playerName) -> nlohmann::json;

	static auto AddNickPlayer(const std::string& playerName) -> void;
	static auto GetNickList() -> std::map<std::string, Nick>&;

	static auto AddAutoGGLine(const std::string& line) -> void;
	static auto GetAutoGGLines() -> std::vector<std::string>;

	static auto GetCurrentGamemode() -> HypixelGamemode::Gamemode;
    static auto SetCurrentGamemode(const HypixelGamemode::Gamemode gamemode) -> void;

	static auto GetCurrentMode() -> std::string;
	static auto SetCurrentMode(const std::string& mode) -> void;

	static auto SetAPIKey(const std::string& key) -> void;

private:
	inline static std::map<std::string, Nick> nickList;

	inline static std::vector<std::string> autoGGLines;

    inline static HypixelGamemode::Gamemode currentGamemode{ HypixelGamemode::Gamemode::LOBBY };

	inline static std::string currentMode = "";

	inline static std::string apiKey{ Config::GetHypixelAPIKey()};
};