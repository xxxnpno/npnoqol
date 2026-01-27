#include "NetworkPlayerInfo.h"

NetworkPlayerInfo::NetworkPlayerInfo(const jobject instance) 
    : JavaClass("net/minecraft/client/network/NetworkPlayerInfo", instance) 
{
    this->Init();
}

NetworkPlayerInfo::~NetworkPlayerInfo() = default;

void NetworkPlayerInfo::Init() 
{
    std::call_once(oflag, [this] 
        {
        getGameProfileMethodID = Jvm::env->GetMethodID(this->javaClass, "getGameProfile", "()Lcom/mojang/authlib/GameProfile;");
        getDisplayNameMethodID = Jvm::env->GetMethodID(this->javaClass, "getDisplayName", "()Lnet/minecraft/util/IChatComponent;");
        setDisplayNameMethodID = Jvm::env->GetMethodID(this->javaClass,"setDisplayName","(Lnet/minecraft/util/IChatComponent;)V");
        });
}

std::unique_ptr<GameProfile> NetworkPlayerInfo::GetGameProfile() const 
{
    return std::make_unique<GameProfile>(Jvm::env->NewGlobalRef(Jvm::env->CallObjectMethod(this->instance, getGameProfileMethodID)));
}

std::unique_ptr<IChatComponent> NetworkPlayerInfo::GetDisplayName() const 
{
    return std::make_unique<IChatComponent>(Jvm::env->NewGlobalRef(Jvm::env->CallObjectMethod(this->instance, getDisplayNameMethodID)));
}

void NetworkPlayerInfo::SetDisplayName(const std::unique_ptr<IChatComponent>& newName)
{
    Jvm::env->CallVoidMethod(this->instance, setDisplayNameMethodID, newName->GetInstance());
}