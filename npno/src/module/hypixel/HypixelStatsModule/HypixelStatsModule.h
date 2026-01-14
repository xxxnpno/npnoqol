#pragma once

#include "../../Module/Module.h"

#include "../../util/HypixelGamemode/HypixelGamemode.h"

#include <string>
#include <utility>
#include <unordered_map>
#include <memory>

namespace hypixel
{
    class HypixelStatsModule : public Module
    {
    public:
        explicit HypixelStatsModule(const bool enable = true, const HypixelGamemode::Gamemode gamemode);

        virtual ~HypixelStatsModule();

        auto SanityCheck() const -> bool override;

        auto GetGamemode() const -> HypixelGamemode:Gamemode;

        static auto GetCurrentGamemode() const -> HypixelGamemode::Gamemode;
        static auto SetCurrentGamemode(const HypixelGamemode::Gamemode gamemode) -> void;

    protected:
        struct Player
        {
            std::string prefix;
            std::string rank;
            std::string suffix;
        };

        auto UpdateTabList() -> void;
        auto UpdateNameTags() -> void;

        virtual auto GetPlayerData(const std::string& playerName) -> Player = 0;

        virtual auto FormatTabName(const std::unique_ptr<EntityPlayer>& player) -> std::string = 0;
        virtual auto FormatNametag(const std::unique_ptr<EntityPlayer>& player) -> std::pair<std::string, std::string> = 0;

        virtual auto GetHpColor(const float hp) const -> std::string = 0;

        mutable std::unordered_map<std::string, Player> playerCache;

        HypixelGamemode::Gamemode gamemode;
    };

    private:
        inline static HypixelGamemode::Gamemode currentGamemode{ HypixelGamemode::Gamemode::LOBBY };
}