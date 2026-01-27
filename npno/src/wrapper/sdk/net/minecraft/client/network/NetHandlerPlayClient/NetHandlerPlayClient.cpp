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

    const jobject collectionLocal = Jvm::env->CallObjectMethod(this->instance, getPlayerInfoMapMethodID);

    const std::unique_ptr<Collection> getTeams = std::make_unique<Collection>(collectionLocal);
    const jobjectArray arrayLocal = static_cast<jobjectArray>(getTeams->ToArray());

    for (jint i = 0; i < getTeams->Size(); ++i)
    {
        const jobject elementLocal = Jvm::env->GetObjectArrayElement(arrayLocal, i);
        playerList.push_back(std::make_unique<NetworkPlayerInfo>(elementLocal));
        Jvm::env->DeleteLocalRef(elementLocal);
    }

    Jvm::env->DeleteLocalRef(arrayLocal);
    Jvm::env->DeleteLocalRef(collectionLocal);

    return playerList;
}