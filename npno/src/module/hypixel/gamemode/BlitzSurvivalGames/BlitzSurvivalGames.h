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
        enum class Mode : I8
        {
            SOLO,
            TEAMS,
            LOBBY
        };

        auto LoadPlayersData(const std::vector<std::string>& playerNames) -> void override;

        auto HandleMode() -> void override;

        auto UpdateChat() const -> void;

        auto FormatTabName(const std::unique_ptr<EntityPlayer>& player) -> std::string override;
        auto FormatNametag(const std::unique_ptr<EntityPlayer>& player) -> std::pair<std::string, std::string> override;

        auto GetWinsColor(const std::string& wins) const -> std::string;
        auto GetKDRColor(const std::string& kdr) const -> std::string;

        auto AssignTeamNumbers() -> void;

        auto GetTeamIndex(const std::string& playerName) const -> I32;
        auto AssignTeamColors() -> void;

        auto GetPlayerTeamName(const std::string& playerName) const -> std::string;
        auto GetPlayerTeamColor(const std::string& playerName) const -> std::string;
        auto GetTeamCount() const -> I32;

        mutable std::unordered_map<std::string, I32> teamNumbers;
        mutable std::unordered_map<std::string, std::string> teamColors;
        bool teamColorsAssigned;

        Mode mode;
    };
}