#include "EntityLivingBase.h"

#include "../../src/wrapper/sdk/java/util/Collection/Collection.h"

EntityLivingBase::EntityLivingBase(const jobject instance)
	: Entity("net/minecraft/entity/EntityLivingBase", instance)
{
	this->Init();
}

EntityLivingBase::EntityLivingBase(const char* name, const jobject instance)
	: Entity(name, instance)
{
	this->Init();
}

EntityLivingBase::~EntityLivingBase() = default;

void EntityLivingBase::Init()
{
	std::call_once(this->entityLivingBaseOFlag, [this]
		{
			getHealthMethodID = Jvm::env->GetMethodID(this->javaClass, "getHealth", "()F");
			getMaxHealthMethodID = Jvm::env->GetMethodID(this->javaClass, "getMaxHealth", "()F");
			getAbsorptionAmountMethodID = Jvm::env->GetMethodID(this->javaClass, " getAbsorptionAmount", "()F");
			getActivePotionEffectsMethodID = Jvm::env->GetMethodID(this->javaClass, "getActivePotionEffects", "()Ljava/util/Collection;");
		});
}

float EntityLivingBase::GetHealth() const
{
	return static_cast<float>(Jvm::env->CallFloatMethod(this->instance, getHealthMethodID));
}

float EntityLivingBase::GetMaxHealth() const
{
	return static_cast<float>(Jvm::env->CallFloatMethod(this->instance, getMaxHealthMethodID));
}

float EntityLivingBase::GetAbsorptionAmount() const
{
	return static_cast<float>(Jvm::env->CallFloatMethod(this->instance, getAbsorptionAmountMethodID));
}

std::vector<std::unique_ptr<PotionEffect>> EntityLivingBase::GetActivePotionEffects() const
{
	std::vector<std::unique_ptr<PotionEffect>> potionList;

	std::unique_ptr<Collection> getActivePotionEffects = std::make_unique<Collection>(Jvm::env->CallObjectMethod(this->instance, getActivePotionEffectsMethodID));
	const jobjectArray array = getActivePotionEffects->ToArray();

	for (jint i = 0; i < getActivePotionEffects->Size(); ++i)
	{
		potionList.push_back(std::make_unique<PotionEffect>(Jvm::env->NewGlobalRef(Jvm::env->GetObjectArrayElement(array, i))));
	}

	Jvm::env->DeleteGlobalRef(array);

	return potionList;
}