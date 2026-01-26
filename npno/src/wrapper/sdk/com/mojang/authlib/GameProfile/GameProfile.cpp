#include "GameProfile.h"

#include "../../src/wrapper/sdk/java/util/Collection/Collection.h"

GameProfile::GameProfile(const jobject instance)
    : JavaClass("com/mojang/authlib/GameProfile", instance)
{
    this->Init();
}

GameProfile::~GameProfile() = default;

void GameProfile::Init()
{
    std::call_once(this->oflag, [this]
        {
            getNameMethodID = Jvm::env->GetMethodID(this->javaClass, "getName", "()Ljava/lang/String;");
            getPropertiesMethodID = Jvm::env->GetMethodID(this->javaClass, "getProperties", "()Lcom/mojang/authlib/properties/PropertyMap;");
        });
}

std::string GameProfile::GetName() const
{
    return JavaUtil::JStringToString(static_cast<jstring>(Jvm::env->CallObjectMethod(this->instance, getNameMethodID)));
}

std::unique_ptr<PropertyMap> GameProfile::GetProperties() const
{
    return std::make_unique<PropertyMap>(Jvm::env->NewGlobalRef(Jvm::env->GetObjectField(this->instance, getProperties)));
}