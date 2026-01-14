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
        auto GetPlayerData(const std::string& playerName) -> Player override;

        auto FormatTabName(const std::unique_ptr<EntityPlayer>& player) -> std::string override;
        auto FormatNametag(const std::unique_ptr<EntityPlayer>& player) -> std::pair<std::string, std::string> override;

        auto GetHpColor(const float hp) const -> std::string override;

        auto GetWinsColor(const std::string& wins) const -> std::string;
        auto GetKDRColor(const std::string& kdr) const -> std::string;
    };
}