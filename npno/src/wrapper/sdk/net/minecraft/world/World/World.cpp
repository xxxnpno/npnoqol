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
            playerEntitiesFieldID = Jvm::env->GetFieldID(this->javaClass, "playerEntities", "Ljava/util/List;");
        });
}

std::vector<std::unique_ptr<EntityPlayer>> World::GetPlayerEntities() const
{
    std::vector<std::unique_ptr<EntityPlayer>> playerList;

    std::unique_ptr<List> playerEntities = std::make_unique<List>(Jvm::env->GetObjectField(this->instance, playerEntitiesFieldID));

    for (jint i = 0; i < playerEntities->Size(); ++i)
    {
        playerList.emplace_back(std::make_unique<EntityPlayer>(playerEntities->Get(i)));
    }

    return playerList;
}