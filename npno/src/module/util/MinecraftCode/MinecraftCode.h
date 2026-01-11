#pragma once

#include <string>
#include <map>

#include <npno/Loader.hpp>

class MinecraftCode final
{
public:
    enum class Code : I8
    {
        BLACK,
        DARK_BLUE,
        DARK_GREEN,
        DARK_AQUA,
        DARK_RED,
        DARK_PURPLE,
        GOLD,
        GRAY,
        DARK_GRAY,
        BLUE,
        GREEN,
        AQUA,
        RED,
        LIGHT_PURPLE,
        YELLOW,
        WHITE,
        OBFUSCATED,
        BOLD,
        STRIKETHROUGH,
        UNDERLINE,
        ITALIC,
        RESET
    };

    inline static const std::map<Code, std::string> codeToString =
    {
        {Code::BLACK, "§0"},
        {Code::DARK_BLUE, "§1"},
        {Code::DARK_GREEN, "§2"},
        {Code::DARK_AQUA, "§3"},
        {Code::DARK_RED, "§4"},
        {Code::DARK_PURPLE, "§5"},
        {Code::GOLD, "§6"},
        {Code::GRAY, "§7"},
        {Code::DARK_GRAY, "§8"},
        {Code::BLUE, "§9"},
        {Code::GREEN, "§a"},
        {Code::AQUA, "§b"},
        {Code::RED, "§c"},
        {Code::LIGHT_PURPLE, "§d"},
        {Code::YELLOW, "§e"},
        {Code::WHITE, "§f"},
        {Code::OBFUSCATED, "§k"},
        {Code::BOLD, "§l"},
        {Code::STRIKETHROUGH, "§m"},
        {Code::UNDERLINE, "§n"},
        {Code::ITALIC, "§o"},
        {Code::RESET, "§r"}
    };
};