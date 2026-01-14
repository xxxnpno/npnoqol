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

        SKYBLITZSURVIVALGAMESWARS
    }

    inline static const std::map<std::string, Gamemode> stringToGamemode = 
    {
        {"LOBBY", Gamemode::LOBBY},
        {"SURVIVAL_GAMES", Gamemode::BLITZSURVIVALGAMES}
    }
}