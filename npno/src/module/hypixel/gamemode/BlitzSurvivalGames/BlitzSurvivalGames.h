#pragma once

#include "../../HypixelStatsModule/HypixelStatsModule.h"

namespace hypixel
{
    class BlitzSurvivalGames final : public HypixelStatsModule
    {
    public:
        BlitzSurvivalGames();

        ~BlitzSurvivalGames() override;

        auto Update() -> void override;

    private:
        enum class Mode
        {
            SOLO,
            TEAMS,
            WAITING,
            LOBBY
        }

        auto LoadPlayersData(const std::vector<std::string>& playerNames) -> void override;

        auto HandleMode() -> void override;

        auto FormatTabName(const std::unique_ptr<EntityPlayer>& player) -> std::string override;
        auto FormatNametag(const std::unique_ptr<EntityPlayer>& player) -> std::pair<std::string, std::string> override;

        auto GetWinsColor(const std::string& wins) const -> std::string;
        auto GetKDRColor(const std::string& kdr) const -> std::string;

        Mode mode;
    };
}