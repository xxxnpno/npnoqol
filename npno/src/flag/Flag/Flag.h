#pragma once 

#include "../../src/wrapper/sdk/net/minecraft/client/Minecraft/Minecraft.h"

#include <chrono>
#include <memory>

#include <npno/Loader.hpp>

class Flag
{
public:
    explicit Flag(const std::string& name);

    virtual ~Flag();

    virtual auto Update() -> void = 0;

    static auto GetPlayers() -> void;

protected:
    struct Player
    {
        std::string name;

        struct
        {
            double x;
            double y;
            double z;
        } position;

        struct
        {
            double x;
            double y;
            double z;
        } velocity;

        float yaw;
        float pitch;

        bool isOnGround;
        std::chrono::time_point<std::chrono::system_clock> onGroundTime;
        std::chrono::time_point<std::chrono::system_clock> timeSinceOnGround;

        bool isSprinting;
        std::chrono::time_point<std::chrono::system_clock> sprintingTime;
        std::chrono::time_point<std::chrono::system_clock> timeSinceSprinted;

        bool isSneaking;
        std::chrono::time_point<std::chrono::system_clock> sneakingTime;
        std::chrono::time_point<std::chrono::system_clock> timeSinceSneaking;

        bool isSwingingSword;
        std::chrono::time_point<std::chrono::system_clock> swingingSwordTime;
        std::chrono::time_point<std::chrono::system_clock> timeSinceSwingingSword;

        bool isBlockingSword;
        std::chrono::time_point<std::chrono::system_clock> blockingSwordTime;
        std::chrono::time_point<std::chrono::system_clock> timeSinceBlockingSword;

        bool isEating;
        std::chrono::time_point<std::chrono::system_clock> isEatingTime;
        std::chrono::time_point<std::chrono::system_clock> timeSinceEating;

        bool isRiding;
        std::chrono::time_point<std::chrono::system_clock> isRidingTime;
        std::chrono::time_point<std::chrono::system_clock> timeSinceRiding;

        bool isBowing;
        std::chrono::time_point<std::chrono::system_clock> isBowingTime;
        std::chrono::time_point<std::chrono::system_clock> timeSinceBowing;
    };

    const std::vector<std::unique_ptr<EntityPlayer>> playerEntities;

    inline static std::unique_ptr<Minecraft> mc{ nullptr };

    std::string name;
};