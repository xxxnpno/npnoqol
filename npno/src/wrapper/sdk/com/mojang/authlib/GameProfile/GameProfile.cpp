#include "GameProfile.h"

GameProfile::GameProfile(const jobject instance)
    : JavaClass("com/mojang/authlib/GameProfile", instance)
{
    this->Init();
}

GameProfile::~GameProfile() = default;

void GameProfile::Init()
{
    std::call_once(oflag, [this]
        {
            getNameMethodID = Jvm::env->GetMethodID(this->javaClass, "getName", "()Ljava/lang/String;");
        });
}

std::string GameProfile::GetName() const
{
    return JavaUtil::JStringToString(static_cast<jstring>(Jvm::env->CallObjectMethod(this->instance, getNameMethodID)));
}