#include "HypixelRank.h"

auto HypixelRank::GetRankPrefix(const nlohmann::json& json) -> std::string
{
    try
    {
        const auto& p = json["player"];
        std::string plusColor = MinecraftCode::codeToString.at(MinecraftCode::Code::RED);

        if (!p["rankPlusColor"].is_null())
        {
            auto it = colorMap.find(p["rankPlusColor"].get<std::string>());
            if (it != colorMap.end())
            {
                plusColor = it->second;
            }
        }

        if (p["monthlyPackageRank"] == "SUPERSTAR")
        {
            std::string rankColor = MinecraftCode::codeToString.at(MinecraftCode::Code::GOLD);
            if (!p["monthlyRankColor"].is_null())
            {
                auto it = colorMap.find(p["monthlyRankColor"].get<std::string>());
                if (it != colorMap.end())
                {
                    rankColor = it->second;
                }
            }
            return rankColor + "[MVP" + plusColor + "++" + rankColor + "] ";
        }

        if (p["newPackageRank"] == "MVP_PLUS")
        {
            const std::string aqua = MinecraftCode::codeToString.at(MinecraftCode::Code::AQUA);
            return aqua + "[MVP" + plusColor + "+" + aqua + "] ";
        }

        if (p["newPackageRank"] == "MVP")
        {
            return MinecraftCode::codeToString.at(MinecraftCode::Code::AQUA) + "[MVP] ";
        }

        if (p["newPackageRank"] == "VIP_PLUS")
        {
            const std::string green = MinecraftCode::codeToString.at(MinecraftCode::Code::GREEN);
            const std::string gold = MinecraftCode::codeToString.at(MinecraftCode::Code::GOLD);
            return green + "[VIP" + gold + "+" + green + "] ";
        }

        if (p["newPackageRank"] == "VIP")
        {
            return MinecraftCode::codeToString.at(MinecraftCode::Code::GREEN) + "[VIP] ";
        }

        return "";
    }
    catch (...)
    {
        return "";
    }
}