#include "World.h"

#include "../../src/wrapper/sdk/java/util/List/List.h"

World::World(const jobject instance)
    : JavaClass("net/minecraft/world/World", instance)
{
    this->Init();
}

World::World(const char* name, const jobject instance)
    : JavaClass(name, instance)
{
    this->Init();
}

World::~World() = default;

void World::Init()
{
    std::call_once(oflag, [this]
        {
            worldInfoFieldID = Jvm::env->GetFieldID(this->javaClass, "worldInfo", "Lnet/minecraft/world/storage/WorldInfo;");
            playerEntitiesFieldID = Jvm::env->GetFieldID(this->javaClass, "playerEntities", "Ljava/util/List;");
        });
}

std::unique_ptr<WorldInfo> World::GetWorldInfo() const
{
    return std::make_unique<WorldInfo>(Jvm::env->NewGlobalRef(Jvm::env->GetObjectField(this->instance, worldInfoFieldID)));
}

std::vector<std::unique_ptr<EntityPlayer>> World::GetPlayerEntities() const
{
    std::vector<std::unique_ptr<EntityPlayer>> playerList;

    const jobject playerEntitiesLocal = Jvm::env->GetObjectField(this->instance, playerEntitiesFieldID);

    const std::unique_ptr<List> playerEntities = std::make_unique<List>(playerEntitiesLocal);

    const jint size = playerEntities->Size();
    for (jint i = 0; i < size; ++i)
    {
        const jobject playerLocal = playerEntities->Get(i);
        playerList.emplace_back(std::make_unique<EntityPlayer>(playerLocal));
        Jvm::env->DeleteLocalRef(playerLocal);
    }

    Jvm::env->DeleteLocalRef(playerEntitiesLocal);
    return playerList;
}