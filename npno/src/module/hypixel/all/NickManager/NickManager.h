#pragma once

#include "../../HypixelStatsModule/HypixelStatsModule.h"

namespace hypixel
{
    class NickManager final : public HypixelStatsModule
    {
    public:
        NickManager();

        ~NickManager() override;

        auto Update() -> void override;

    private:
        auto Warn(const std::string& name, const std::string& realName) -> void;

        auto SkinDenicker() -> void;

        auto ParseSkinData(const std::string& playerName, const std::string& skinHash, const std::string& profileName) -> void;
        auto IsNickHash(const std::string& hash) -> bool;
        
        std::set<std::string> nickHashes;
        std::set<std::string> parsedPlayers;
    };
}