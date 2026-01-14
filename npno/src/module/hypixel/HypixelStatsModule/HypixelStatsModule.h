#pragma once

#include "../../Module/Module.h"

#include "../../util/HypixelGamemode/HypixelGamemode.h"

#include <string>
#include <utility>
#include <unordered_map>
#include <memory>

#include <nlohmann/json.hpp>

namespace hypixel
{
    class HypixelStatsModule : public Module
    {
    public:
        explicit HypixelStatsModule(const bool enable = true, const HypixelGamemode::Gamemode gamemode =  HypixelGamemode::Gamemode::ALL, const std::string& autoGGLine = "idontwantittoprocsoiputarandomstring");

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
        };

        auto UpdateTabList() -> void;
        auto UpdateNameTags() -> void;

        virtual auto GetPlayerData(const std::string& playerName) -> Player { return { "", "", "" }; };

        virtual auto FormatTabName(const std::unique_ptr<EntityPlayer>& player) -> std::string { return ""; };
        virtual auto FormatNametag(const std::unique_ptr<EntityPlayer>& player) -> std::pair<std::string, std::string> { return { "", "" }; };

        virtual auto GetHpColor(const float hp) const -> std::string { return ""; };

        mutable std::unordered_map<std::string, Player> playerCache;

        HypixelGamemode::Gamemode gamemode;
    };
}