#pragma once

#include "../MinecraftCode/MinecraftCode.h"

#include <nlohmann/json.hpp>

class HypixelRank final
{
public:
    static auto GetRankPrefix(const nlohmann::json& json) -> std::string;

private:
    inline static const std::map<std::string, std::string> colorMap =
    {
        {"BLACK", MinecraftCode::codeToString.at(MinecraftCode::Code::BLACK)},
        {"DARK_BLUE", MinecraftCode::codeToString.at(MinecraftCode::Code::DARK_BLUE)},
        {"DARK_GREEN", MinecraftCode::codeToString.at(MinecraftCode::Code::DARK_GREEN)},
        {"DARK_AQUA", MinecraftCode::codeToString.at(MinecraftCode::Code::DARK_AQUA)},
        {"DARK_RED", MinecraftCode::codeToString.at(MinecraftCode::Code::DARK_RED)},
        {"DARK_PURPLE", MinecraftCode::codeToString.at(MinecraftCode::Code::DARK_PURPLE)},
        {"GOLD", MinecraftCode::codeToString.at(MinecraftCode::Code::GOLD)},
        {"GRAY", MinecraftCode::codeToString.at(MinecraftCode::Code::GRAY)},
        {"DARK_GRAY", MinecraftCode::codeToString.at(MinecraftCode::Code::DARK_GRAY)},
        {"BLUE", MinecraftCode::codeToString.at(MinecraftCode::Code::BLUE)},
        {"GREEN", MinecraftCode::codeToString.at(MinecraftCode::Code::GREEN)},
        {"AQUA", MinecraftCode::codeToString.at(MinecraftCode::Code::AQUA)},
        {"RED", MinecraftCode::codeToString.at(MinecraftCode::Code::RED)},
        {"LIGHT_PURPLE", MinecraftCode::codeToString.at(MinecraftCode::Code::LIGHT_PURPLE)},
        {"YELLOW", MinecraftCode::codeToString.at(MinecraftCode::Code::YELLOW)},
        {"WHITE", MinecraftCode::codeToString.at(MinecraftCode::Code::WHITE)}
    };
};