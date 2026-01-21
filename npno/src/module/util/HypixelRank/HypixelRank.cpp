#include "HypixelRank.h"

auto HypixelRank::GetRankPrefix(const nlohmann::json& json) -> std::string
{
    const auto playerIt = json.find("player");
    if (playerIt == json.end() or !playerIt->is_object())
    {
        return "";
    }

    const auto& p = *playerIt;

    auto getColor = [](MinecraftCode::Code code) -> std::string
        {
            auto it = MinecraftCode::codeToString.find(code);
            return (it != MinecraftCode::codeToString.end()) ? it->second : "";
        };

    std::string plusColor = getColor(MinecraftCode::Code::RED);

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
        std::string rankColor = getColor(MinecraftCode::Code::GOLD);

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
        std::string aqua = getColor(MinecraftCode::Code::AQUA);
        return aqua + "[MVP" + plusColor + "+" + aqua + "]";
    }

    if (packageRank == "MVP")
    {
        std::string aqua = getColor(MinecraftCode::Code::AQUA);
        return aqua + "[MVP]";
    }

    if (packageRank == "VIP_PLUS")
    {
        std::string green = getColor(MinecraftCode::Code::GREEN);
        std::string gold = getColor(MinecraftCode::Code::GOLD);
        return green + "[VIP" + gold + "+" + green + "]";
    }

    if (packageRank == "VIP")
    {
        std::string green = getColor(MinecraftCode::Code::GREEN);
        return green + "[VIP]";
    }

    return "";
}