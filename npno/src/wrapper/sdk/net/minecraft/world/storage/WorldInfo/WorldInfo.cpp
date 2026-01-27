#include "WorldInfo.h"

#include "../../src/wrapper/sdk/java/util/List/List.h"

WorldInfo::WorldInfo(const jobject instance)
    : JavaClass("net/minecraft/world/storage/WorldInfo", instance)
{
    this->Init();
}

WorldInfo::WorldInfo(const char* name, const jobject instance)
    : JavaClass(name, instance)
{
    this->Init();
}

WorldInfo::~WorldInfo() = default;

void WorldInfo::Init()
{
    std::call_once(oflag, [this]
        {
            randomSeedFieldID = Jvm::env->GetFieldID(this->javaClass, "randomSeed", "J");
            worldTimeFieldID = Jvm::env->GetFieldID(this->javaClass, "worldTime", "J");
            worldNameFieldID = Jvm::env->GetFieldID(this->javaClass, "worldName", "Ljava/lang/String;");
        });
}

I64 WorldInfo::GetRandomSeed() const
{
    return static_cast<I64>(Jvm::env->GetLongField(this->instance, randomSeedFieldID));
}

I64 WorldInfo::GetWorldTime() const
{
    return static_cast<I64>(Jvm::env->GetLongField(this->instance, worldTimeFieldID));
}

std::string WorldInfo::GetWorldName() const
{
    return JavaUtil::JStringToString(static_cast<jstring>(Jvm::env->GetObjectField(this->instance, worldNameFieldID)));
}