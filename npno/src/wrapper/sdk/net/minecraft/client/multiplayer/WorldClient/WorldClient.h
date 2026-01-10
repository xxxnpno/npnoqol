#pragma once

#include "../../src/wrapper/sdk/net/minecraft/world/World/World.h"

#include "../../src/wrapper/sdk/net/minecraft/entity/player/EntityPlayer/EntityPlayer.h"

class WorldClient final : public World
{
public:
	explicit WorldClient(const jobject instance);

	virtual ~WorldClient() override;

	virtual void Init() override;

	[[nodiscard]] std::unique_ptr<EntityPlayer> GetPlayerEntityByName(const std::string& name) const;

private:
	inline static std::once_flag worldClientOflag;

	inline static jmethodID getPlayerEntityByNameMethodID{ nullptr };
};