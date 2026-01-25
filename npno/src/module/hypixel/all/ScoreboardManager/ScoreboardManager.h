#pragma once

#include "../../HypixelStatsModule/HypixelStatsModule.h"

namespace hypixel
{
    class ScoreboardManager final : public HypixelStatsModule
    {
    public:
        ScoreboardManager();

        ~ScoreboardManager() override;

        auto Update() -> void override;
    };
}