#pragma once

#include "../../src/wrapper/sdk/net/minecraft/entity/EntityLivingBase/EntityLivingBase.h"

class EntityPlayer : public EntityLivingBase
{
public:
	explicit EntityPlayer(const jobject instance);
	EntityPlayer(const char* name, const jobject instance);

	virtual ~EntityPlayer() override;

	virtual void Init() override;

	[[nodiscard]] std::string GetCustomNameTag() const;

private:
	inline static std::once_flag entityPlayerSPOflag;

	inline static jmethodID getCustomNameTagMethodID{ nullptr };
};