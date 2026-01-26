#pragma once

#include "../../src/wrapper/sdk/net/minecraft/entity/EntityLivingBase/EntityLivingBase.h"

#include "../../src/wrapper/sdk/com/mojang/authlib/GameProfile/GameProfile.h"

class EntityPlayer : public EntityLivingBase
{
public:
	explicit EntityPlayer(const jobject instance);
	EntityPlayer(const char* name, const jobject instance);

	virtual ~EntityPlayer() override;

	virtual void Init() override;

	[[nodiscard]] virtual bool IsSpectator() const final;

	[[nodiscard]] virtual bool CanAttackPlayer(const std::unique_ptr<EntityPlayer>& target) const final;

	[[nodiscard]] virtual std::string GetCustomNameTag() const final;

	[[nodiscard]] std::unique_ptr<GameProfile> GetGameProfile() const;

private:
	inline static std::once_flag oflag{};

	inline static jmethodID isSpectatorMethodID{ nullptr };
	inline static jmethodID canAttackPlayerMethodID{ nullptr };
	inline static jmethodID getCustomNameTagMethodID{ nullptr };
	inline static jmethodID getGameProfileMethodID{ nullptr };
};