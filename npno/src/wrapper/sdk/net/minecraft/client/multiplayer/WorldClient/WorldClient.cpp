#include "WorldClient.h"

WorldClient::WorldClient(const jobject instance)
    : World("net/minecraft/client/multiplayer/WorldClient", instance)
{
    this->Init();
}

WorldClient::~WorldClient() = default;

void WorldClient::Init()
{
    std::call_once(this->worldClientOflag, [this]
        {
            getPlayerEntityByNameMethodID = Jvm::env->GetMethodID(this->javaClass, "getPlayerEntityByName", "(Ljava/lang/String;)Lnet/minecraft/entity/player/EntityPlayer;"
            );
        });
}

std::unique_ptr<EntityPlayer> WorldClient::GetPlayerEntityByName(const std::string& name) const
{
    return std::make_unique<EntityPlayer>(Jvm::env->NewGlobalRef(Jvm::env->CallObjectMethod(this->instance, getPlayerEntityByNameMethodID, JavaUtil::StringToJString(name))));
}