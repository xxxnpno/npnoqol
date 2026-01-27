#include "NetHandlerPlayClient.h"

#include "../../src/wrapper/sdk/java/util/Collection/Collection.h"

NetHandlerPlayClient::NetHandlerPlayClient(const jobject instance) 
    : JavaClass("net/minecraft/client/network/NetHandlerPlayClient", instance) 
{
    this->Init();
}

NetHandlerPlayClient::~NetHandlerPlayClient() = default;

void NetHandlerPlayClient::Init()
{
    std::call_once(oflag, [this] 
        {
        getPlayerInfoMapMethodID = Jvm::env->GetMethodID(this->javaClass, "getPlayerInfoMap", "()Ljava/util/Collection;");
        });
}

std::vector<std::unique_ptr<NetworkPlayerInfo>> NetHandlerPlayClient::GetPlayerInfoMap() const 
{
    std::vector<std::unique_ptr<NetworkPlayerInfo>> playerList;

    std::unique_ptr<Collection> getPlayerInfoMap = std::make_unique<Collection>(Jvm::env->CallObjectMethod(this->instance, getPlayerInfoMapMethodID));

    const jobjectArray array = getPlayerInfoMap->ToArray();

    for (jint i = 0; i < getPlayerInfoMap->Size(); ++i)
    {
        playerList.push_back(std::make_unique<NetworkPlayerInfo>(Jvm::env->NewGlobalRef(Jvm::env->GetObjectArrayElement(array, i))));
    }

    Jvm::env->DeleteGlobalRef(array);

    return playerList;
}