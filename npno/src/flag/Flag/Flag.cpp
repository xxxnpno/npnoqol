#include "Flag.h"

#include <algorithm>

Flag::Flag(const std::string& name)
    : name{ name }
{
    static std::once_flag oflag;
    std::call_once(oflag, []
        {
            mc = std::make_unique<Minecraft>();
        });
}

Flag::~Flag() = default;

auto Flag::GetPlayers() -> void
{
    if (!mc->GetTheWorld()->GetInstance())
    {
        playersLastTick.clear();
        return;
    }

    const std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    const std::vector<std::unique_ptr<EntityPlayer>>& playerEntities = mc->GetTheWorld()->GetPlayerEntities();

    playersLastTick.erase(std::remove_if(playersLastTick.begin(), playersLastTick.end(),
        [&playerEntities](const Player& p)
        {
            return std::none_of(playerEntities.begin(), playerEntities.end(),
                [&p](const std::unique_ptr<EntityPlayer>& player)
                {
                    return player->GetName() == p.name;
                }
            );
        }
    ),
        playersLastTick.end()
    );

    for (const std::unique_ptr<EntityPlayer>& player : playerEntities)
    {
        auto it = std::find_if(playersLastTick.begin(), playersLastTick.end(),
            [&player](const Player& p)
            {
                return p.name == player->GetName();
            }
        );

        const std::string name = player->GetName();

        const double posX = player->GetPosX();
        const double posY = player->GetPosY();
        const double posZ = player->GetPosZ();

        const double velocityX = player->GetMotionX();
        const double velocityY = player->GetMotionY();
        const double velocityZ = player->GetMotionZ();

        const float rotationYaw = player->GetRotationYaw();
        const float rotationPitch = player->GetRotationPitch();

        const bool isOnGround = player->IsOnGround();
        const bool isSprinting = player->IsSprinting();
        const bool isSneaking = player->IsSneaking();
        const bool isSwingingSword = player->IsUsingItem() and false;
        const bool isBlockingSword = false;
        const bool isEating = player->IsEating();
        const bool isRiding = player->IsRiding();
        const bool isBowing = player->IsUsingItem() and false;

        if (it == playersLastTick.end())
        {
            Player p{};

            p.name = name;

            p.position.x = posX;
            p.position.y = posY;
            p.position.z = posZ;

            p.velocity.x = velocityX;
            p.velocity.y = velocityY;
            p.velocity.z = velocityZ;

            p.yaw = rotationYaw;
            p.pitch = rotationPitch;

            p.isOnGround = isOnGround;
            p.onGroundTime = isOnGround ? now : std::chrono::time_point<std::chrono::system_clock>{};
            p.timeSinceOnGround = !isOnGround ? now : std::chrono::time_point<std::chrono::system_clock>{};

            p.isSprinting = isSprinting;
            p.sprintingTime = isSprinting ? now : std::chrono::time_point<std::chrono::system_clock>{};
            p.timeSinceSprinted = !isSprinting ? now : std::chrono::time_point<std::chrono::system_clock>{};

            p.isSneaking = isSneaking;
            p.sneakingTime = isSneaking ? now : std::chrono::time_point<std::chrono::system_clock>{};
            p.timeSinceSneaking = !isSneaking ? now : std::chrono::time_point<std::chrono::system_clock>{};

            p.isSwingingSword = isSwingingSword;
            p.swingingSwordTime = isSwingingSword ? now : std::chrono::time_point<std::chrono::system_clock>{};
            p.timeSinceSwingingSword = !isSwingingSword ? now : std::chrono::time_point<std::chrono::system_clock>{};

            p.isBlockingSword = isBlockingSword;
            p.blockingSwordTime = isBlockingSword ? now : std::chrono::time_point<std::chrono::system_clock>{};
            p.timeSinceBlockingSword = !isBlockingSword ? now : std::chrono::time_point<std::chrono::system_clock>{};

            p.isEating = isEating;
            p.isEatingTime = isEating ? now : std::chrono::time_point<std::chrono::system_clock>{};
            p.timeSinceEating = !isEating ? now : std::chrono::time_point<std::chrono::system_clock>{};

            p.isRiding = isRiding;
            p.isRidingTime = isRiding ? now : std::chrono::time_point<std::chrono::system_clock>{};
            p.timeSinceRiding = !isRiding ? now : std::chrono::time_point<std::chrono::system_clock>{};

            p.isBowing = isBowing;
            p.isBowingTime = isBowing ? now : std::chrono::time_point<std::chrono::system_clock>{};
            p.timeSinceBowing = !isBowing ? now : std::chrono::time_point<std::chrono::system_clock>{};

            playersLastTick.push_back(p);
        }
        else
        {
            it->name = name;

            it->position.x = posX;
            it->position.y = posY;
            it->position.z = posZ;

            it->velocity.x = velocityX;
            it->velocity.y = velocityY;
            it->velocity.z = velocityZ;

            it->yaw = rotationYaw;
            it->pitch = rotationPitch;

            if (it->isOnGround != isOnGround)
            {
                if (isOnGround)
                {
                    it->onGroundTime = now;
                }
                else
                {
                    it->timeSinceOnGround = now;
                }
            }
            it->isOnGround = isOnGround;

            if (it->isSprinting != isSprinting)
            {
                if (isSprinting)
                {
                    it->sprintingTime = now;
                }
                else
                {
                    it->timeSinceSprinted = now;
                }
            }
            it->isSprinting = isSprinting;

            if (it->isSneaking != isSneaking)
            {
                if (isSneaking)
                {
                    it->sneakingTime = now;
                }
                else
                {
                    it->timeSinceSneaking = now;
                }
            }
            it->isSneaking = isSneaking;

            if (it->isSwingingSword != isSwingingSword)
            {
                if (isSwingingSword)
                {
                    it->swingingSwordTime = now;
                }
                else
                {
                    it->timeSinceSwingingSword = now;
                }
            }
            it->isSwingingSword = isSwingingSword;

            if (it->isBlockingSword != isBlockingSword)
            {
                if (isBlockingSword)
                {
                    it->blockingSwordTime = now;
                }
                else
                {
                    it->timeSinceBlockingSword = now;
                }
            }
            it->isBlockingSword = isBlockingSword;

            if (it->isEating != isEating)
            {
                if (isEating)
                {
                    it->isEatingTime = now;
                }
                else
                {
                    it->timeSinceEating = now;
                }
            }
            it->isEating = isEating;

            if (it->isRiding != isRiding)
            {
                if (isRiding)
                {
                    it->isRidingTime = now;
                }
                else
                {
                    it->timeSinceRiding = now;
                }
            }
            it->isRiding = isRiding;

            if (it->isBowing != isBowing)
            {
                if (isBowing)
                {
                    it->isBowingTime = now;
                }
                else
                {
                    it->timeSinceBowing = now;
                }
            }
            it->isBowing = isBowing;
        }
    }
}

auto Flag::GetLastTickPlayers() -> std::vector<Player>
{
    return playersLastTick;
}