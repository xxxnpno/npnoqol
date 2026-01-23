#pragma once

#include "../../HypixelStatsModule/HypixelStatsModule.h"

namespace hypixel
{
    class AutoGG final : public HypixelStatsModule
    {
    public:
        AutoGG();

        ~AutoGG() override;

        auto Update() -> void override;

    private:
        auto RandomCase(const std::string& message) const -> std::string;
    };
}