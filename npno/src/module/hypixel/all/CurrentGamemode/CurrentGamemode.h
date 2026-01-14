#pragma once

#include "../../HypixelStatsModule/HypixelStatsModule.h"

namespace hypixel
{
    class CurrentGamemode final : public HypixelStatsModule
    {
    public:
        CurrentGamemode();

        ~CurrentGamemode() override;

        auto Update() -> void override;

    private:
        auto AddChatMessagehook(jthread thread) const -> void;
    };
}