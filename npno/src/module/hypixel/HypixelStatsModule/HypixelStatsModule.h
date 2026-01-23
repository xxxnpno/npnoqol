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
    class HypixelStatsModule : public Module
    {
    public:
        explicit HypixelStatsModule(const bool enable = true, const HypixelGamemode::Gamemode gamemode = HypixelGamemode::Gamemode::ALL, const std::string& autoGGLine = "", const std::string& gameStartsMessage = "");

        virtual ~HypixelStatsModule();

        auto SanityCheck() const -> bool override;

        auto GetGamemode() const -> HypixelGamemode::Gamemode;

        auto ClearCache() -> void;

    protected:
        enum class ModeState : I8
        {
            INGAME,
            PREGAMEANDRELOADED,
            PREGAME,
            NOTINGAME
        };

        struct Player
        {
            std::string prefix = "";
            std::string rank = "";
            std::string suffix = "";

            bool isNick = false;
            bool error = false;
        };

        struct Team
        {
            std::string playerName = "";
            std::string hypixelTeam = "";
            std::string npnoTeam = "";
        };

        auto UpdateTabList() -> void;
        auto UpdateNameTags() -> void;

        auto IsBot(const std::unique_ptr<EntityPlayer>& player) -> bool;

        virtual auto GetPlayerData(const std::string& playerName) -> Player;

        virtual auto LoadPlayersData(const std::vector<std::string>& playerNames) -> void {};

        virtual auto LoadMissingPlayers() -> void;

        virtual auto FormatTabName(const std::unique_ptr<EntityPlayer>& player) -> std::string { return ""; };
        virtual auto FormatNametag(const std::unique_ptr<EntityPlayer>& player) -> std::pair<std::string, std::string> { return { "", "" }; };

        virtual auto GetHpColor(const float hp) const -> std::string;

        virtual auto HandleMode() -> void {};

        virtual auto GetTeamFromTeamManager(const std::string& playerName) const final -> Team;

        virtual	auto SentByServer(const std::string& line) const final -> bool;

        mutable std::unordered_map<std::string, Player> playerCache;

        mutable std::set<std::string> loadingPlayers;

        mutable std::vector<Team> teamManager;

        HypixelGamemode::Gamemode gamemode;

        ModeState modeState = ModeState::NOTINGAME;

    private:
        auto HandleGameStart() -> void;

        std::string gameStartsMessage;
    };
}