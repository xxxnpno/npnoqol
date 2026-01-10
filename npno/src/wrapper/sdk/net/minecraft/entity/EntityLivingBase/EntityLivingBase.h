#pragma once

#include "../../src/wrapper/sdk/net/minecraft/entity/Entity/Entity.h"

#include "../../src/wrapper/sdk/net/minecraft/potion/PotionEffect/PotionEffect.h"

class EntityLivingBase : public Entity
{
public:
	explicit EntityLivingBase(const jobject instance);
	EntityLivingBase(const char* name, const jobject instance);

	virtual ~EntityLivingBase() override;

	virtual void Init() override;

	[[nodiscard]] virtual float GetHealth() const final;
	[[nodiscard]] virtual float GetMaxHealth() const final;

	[[nodiscard]] virtual std::vector<std::unique_ptr<PotionEffect>> GetActivePotionEffects() const final;

private:
	inline static std::once_flag entityLivingBaseOFlag;

	inline static jmethodID getHealthMethodID{ nullptr };
	inline static jmethodID getMaxHealthMethodID{ nullptr };
	inline static jmethodID getActivePotionEffectsMethodID{ nullptr };
};