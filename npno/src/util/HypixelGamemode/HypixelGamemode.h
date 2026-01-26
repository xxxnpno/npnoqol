#pragma once

#include <string>
#include <map>

#include <npno/Loader.hpp>

class HypixelGamemode final
{
public:
    enum class Gamemode : I8
    {
        ALL,
        LOBBY,
        QUAKECRAFT,
        WALLS,
        PAINTBALL,
        BLITZSURVIVALGAMES,
        TNTGAMES,
        VAMPIREZ,
        WALLS3,
        ARCADE,
        ARENA,
        UHC,
        MCGO,
        BATTLEGROUND,
        SUPER_SMASH,
        GINGERBREAD,
        HOUSING,
        SKYWARS,
        TRUE_COMBAT,
        SPEED_UHC,
        SKYCLASH,
        LEGACY,
        PROTOTYPE,
        BEDWARS,
        MURDER_MYSTERY,
        BUILD_BATTLE,
        DUELS,
        SKYBLOCK,
        PIT,
        REPLAY,
        SMP,
        WOOL_GAMES
    };

    inline static const std::map<std::string, Gamemode> stringToGamemode =
    {
        {"ALL", Gamemode::ALL},
        {"LOBBY", Gamemode::LOBBY},
        {"QUAKECRAFT", Gamemode::QUAKECRAFT},
        {"WALLS", Gamemode::WALLS},
        {"PAINTBALL", Gamemode::PAINTBALL},
        {"SURVIVAL_GAMES", Gamemode::BLITZSURVIVALGAMES},
        {"TNTGAMES", Gamemode::TNTGAMES},
        {"VAMPIREZ", Gamemode::VAMPIREZ},
        {"WALLS3", Gamemode::WALLS3},
        {"ARCADE", Gamemode::ARCADE},
        {"ARENA", Gamemode::ARENA},
        {"UHC", Gamemode::UHC},
        {"MCGO", Gamemode::MCGO},
        {"BATTLEGROUND", Gamemode::BATTLEGROUND},
        {"SUPER_SMASH", Gamemode::SUPER_SMASH},
        {"GINGERBREAD", Gamemode::GINGERBREAD},
        {"HOUSING", Gamemode::HOUSING},
        {"SKYWARS", Gamemode::SKYWARS},
        {"TRUE_COMBAT", Gamemode::TRUE_COMBAT},
        {"SPEED_UHC", Gamemode::SPEED_UHC},
        {"SKYCLASH", Gamemode::SKYCLASH},
        {"LEGACY", Gamemode::LEGACY},
        {"PROTOTYPE", Gamemode::PROTOTYPE},
        {"BEDWARS", Gamemode::BEDWARS},
        {"MURDER_MYSTERY", Gamemode::MURDER_MYSTERY},
        {"BUILD_BATTLE", Gamemode::BUILD_BATTLE},
        {"DUELS", Gamemode::DUELS},
        {"SKYBLOCK", Gamemode::SKYBLOCK},
        {"PIT", Gamemode::PIT},
        {"REPLAY", Gamemode::REPLAY},
        {"SMP", Gamemode::SMP},
        {"WOOL_GAMES", Gamemode::WOOL_GAMES}
    };
};