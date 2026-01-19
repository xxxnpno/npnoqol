#pragma once

#include "../../Module/Module.h"

#include "../../util/HypixelGamemode/HypixelGamemode.h"

#include <string>
#include <utility>
#include <set>
#include <unordered_map>
#include <memory>

#include <nlohmann/json.hpp>

namespace hypixel
{
    class HypixelStatsModule : public Module
    {
    public:
        explicit HypixelStatsModule(const bool enable = true, const HypixelGamemode::Gamemode gamemode = HypixelGamemode::Gamemode::ALL, const std::string& autoGGLine = "idontwantittoprocsoiputarandomstring");

        virtual ~HypixelStatsModule();

        auto SanityCheck() const -> bool override;

        auto GetGamemode() const -> HypixelGamemode::Gamemode;

        auto ClearCache() -> void;

    protected:
        struct Player
        {
            std::string prefix;
            std::string rank;
            std::string suffix;
            bool isNick = false;
            bool error = false;
        };

        auto UpdateTabList() -> void;
        auto UpdateNameTags() -> void;

        auto IsBot(const std::unique_ptr<EntityPlayer>& player) -> bool;

        virtual auto GetPlayerData(const std::string& playerName) -> Player { return { "", "", "", false, false }; };

        virtual auto LoadPlayersData(const std::vector<std::string>& playerNames) -> void {};

        virtual auto FormatTabName(const std::unique_ptr<EntityPlayer>& player) -> std::string { return ""; };
        virtual auto FormatNametag(const std::unique_ptr<EntityPlayer>& player) -> std::pair<std::string, std::string> { return { "", "" }; };

        virtual auto GetHpColor(const float hp) const -> std::string { return ""; };

        mutable std::unordered_map<std::string, Player> playerCache;

        mutable std::set<std::string> loadingPlayers;

        HypixelGamemode::Gamemode gamemode;
    };
}