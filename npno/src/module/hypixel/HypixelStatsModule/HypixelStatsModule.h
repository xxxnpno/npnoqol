#pragma once

#include "../../Module/Module.h"
#include "../../util/HypixelGamemode/HypixelGamemode.h"
#include "../../util/HypixelRank/HypixelRank.h"
#include "../../util/api/HypixelAPI/HypixelAPI.h"
#include "../../util/MinecraftCode/MinecraftCode.h"

#include <cmath>
#include <string>
#include <vector>
#include <format>
#include <utility>
#include <algorithm>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <memory>

#include <nlohmann/json.hpp>

namespace hypixel
{
    struct Team
    {
        std::vector<std::string> members;
        I32 teamNumber;
    };

    class HypixelStatsModule : public Module
    {
    public:
        explicit HypixelStatsModule(const bool enable = true, const HypixelGamemode::Gamemode gamemode = HypixelGamemode::Gamemode::ALL, const std::string& autoGGLine = "idontwantittoprocsoiputarandomstring");

        virtual ~HypixelStatsModule();

        auto SanityCheck() const -> bool override;

        auto GetGamemode() const -> HypixelGamemode::Gamemode;

        auto ClearCache() -> void;

    protected:
        enum class GameState
        {
            LOBBY,
            PREGAME,
            INGAME
        };

        struct Player
        {
            std::string prefix = "";
            std::string rank = "";
            std::string suffix = "";

            bool isNick = false;
            bool error = false;
        };

        auto UpdateTabList() -> void;
        auto UpdateNameTags() -> void;

        auto IsBot(const std::unique_ptr<EntityPlayer>& player) -> bool;

        virtual auto GetPlayerData(const std::string& playerName) -> Player;

        virtual auto LoadPlayersData(const std::vector<std::string>& playerNames) -> void {};

        virtual auto IsEveryoneLoaded() -> bool;

        virtual auto FormatTabName(const std::unique_ptr<EntityPlayer>& player) -> std::string { return ""; };
        virtual auto FormatNametag(const std::unique_ptr<EntityPlayer>& player) -> std::pair<std::string, std::string> { return { "", "" }; };

        virtual auto GetHpColor(const float hp) const -> std::string;

        virtual auto HandleMode() -> void {};

        auto DetectTeams(const std::vector<std::unique_ptr<EntityPlayer>>& players) -> void;
        auto CheckGameStart(const std::vector<std::unique_ptr<EntityPlayer>>& players) -> bool;
        auto GetPlayerTeamNumber(const std::string& playerName) const -> I32;
        auto ResetTeams() -> void;
        auto GetTeamPrefix(const std::string& playerName) const -> std::string;

        mutable std::unordered_map<std::string, Player> playerCache;
        mutable std::set<std::string> loadingPlayers;

        GameState gameState{ GameState::LOBBY };
        std::unordered_map<std::string, I32> playerToTeam;
        std::vector<Team> teams;
        bool teamsDetected{ false };
        std::unordered_map<std::string, std::string> playerToScoreboardTeam;
        I32 nextTeamNumber{ 1 };

        HypixelGamemode::Gamemode gamemode;
    };
}