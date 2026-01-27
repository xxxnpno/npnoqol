#pragma once

#include "../../HypixelStatsModule/HypixelStatsModule.h"

namespace hypixel
{
    class ChatManager final : public HypixelStatsModule
    {
    public:
        ChatManager();

        ~ChatManager() override;

        auto Update() -> void override;

    private:
        auto AddBlacklistedLine(const std::string& line) -> void;
        auto AddBlacklistedLines(const std::vector<std::string>& lines) -> void;

        std::vector<std::string> blacklistedLines;
    };
}