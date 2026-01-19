#include "HypixelRank.h"

auto HypixelRank::GetRankPrefix(const nlohmann::json& json) -> std::string
{
    const auto playerIt = json.find("player");
    if (playerIt == json.end() || !playerIt->is_object())
    {
        return "";
    }

    const auto& p = *playerIt;

    std::string plusColor;
    if (auto it = MinecraftCode::codeToString.find(MinecraftCode::Code::RED); it != MinecraftCode::codeToString.end())
    {
        plusColor = it->second;
    }

    if (p.contains("rankPlusColor") && p["rankPlusColor"].is_string())
    {
        auto it = colorMap.find(p["rankPlusColor"].get<std::string>());
        if (it != colorMap.end())
        {
            plusColor = it->second;
        }
    }

    std::string monthlyRank = p.value("monthlyPackageRank", "");
    std::string packageRank = p.value("newPackageRank", "");

    if (monthlyRank == "SUPERSTAR")
    {
        std::string rankColor;
        if (auto it = MinecraftCode::codeToString.find(MinecraftCode::Code::GOLD); it != MinecraftCode::codeToString.end())
        {
            rankColor = it->second;
        }

        if (p.contains("monthlyRankColor") && p["monthlyRankColor"].is_string())
        {
            auto it = colorMap.find(p["monthlyRankColor"].get<std::string>());
            if (it != colorMap.end())
            {
                rankColor = it->second;
            }
        }

        return rankColor + "[MVP" + plusColor + "++" + rankColor + "]";
    }

    if (packageRank == "MVP_PLUS")
    {
        auto it = MinecraftCode::codeToString.find(MinecraftCode::Code::AQUA);
        if (it != MinecraftCode::codeToString.end())
        {
            return it->second + "[MVP" + plusColor + "+" + it->second + "]";
        }
        return "";
    }

    if (packageRank == "MVP")
    {
        auto it = MinecraftCode::codeToString.find(MinecraftCode::Code::AQUA);
        if (it != MinecraftCode::codeToString.end())
        {
            return it->second + "[MVP]";
        }
        return "";
    }

    if (packageRank == "VIP_PLUS")
    {
        auto greenIt = MinecraftCode::codeToString.find(MinecraftCode::Code::GREEN);
        auto goldIt = MinecraftCode::codeToString.find(MinecraftCode::Code::GOLD);

        if (greenIt != MinecraftCode::codeToString.end() && goldIt != MinecraftCode::codeToString.end())
        {
            return greenIt->second + "[VIP" + goldIt->second + "+" + greenIt->second + "]";
        }

        return "";
    }

    if (packageRank == "VIP")
    {
        auto it = MinecraftCode::codeToString.find(MinecraftCode::Code::GREEN);
        if (it != MinecraftCode::codeToString.end())
        {
            return it->second + "[VIP]";
        }
    }

    return "";
}