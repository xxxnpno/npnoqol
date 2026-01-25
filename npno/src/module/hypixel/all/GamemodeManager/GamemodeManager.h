#pragma once

#include "../../HypixelStatsModule/HypixelStatsModule.h"

namespace hypixel
{
    class GamemodeManager final : public HypixelStatsModule
    {
    public:
        GamemodeManager();

        ~GamemodeManager() override;

        auto Update() -> void override;

    private:
        auto ExtractJson(const std::string& line) -> std::string;
    };
}