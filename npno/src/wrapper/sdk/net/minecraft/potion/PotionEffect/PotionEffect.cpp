#include "PotionEffect.h"

PotionEffect::PotionEffect(const jobject instance)
    : JavaClass("net/minecraft/potion/PotionEffect", instance)
{
    this->Init();
}

PotionEffect::~PotionEffect() = default;

void PotionEffect::Init()
{
    std::call_once(this->oflag, [this]
        {
            getDurationMethodID = Jvm::env->GetMethodID(this->javaClass, "getDuration", "()I");
            getAmplifierMethodID = Jvm::env->GetMethodID(this->javaClass, "getAmplifier", "()I");
            getEffectNameMethodID = Jvm::env->GetMethodID(this->javaClass, "getEffectName", "()Ljava/lang/String;");
        });
}

I32 PotionEffect::GetDuration() const
{
    return static_cast<I32>(Jvm::env->CallIntMethod(this->instance, getDurationMethodID));
}

I32 PotionEffect::GetAmplifier() const
{
    return static_cast<I32>(Jvm::env->CallIntMethod(this->instance, getAmplifierMethodID));
}

std::string PotionEffect::GetEffectName() const
{
    return JavaUtil::JStringToString(static_cast<jstring>(Jvm::env->CallObjectMethod(this->instance, getEffectNameMethodID)));
}