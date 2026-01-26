#include "Minecraft.h"

Minecraft::Minecraft()
    : JavaClass("net/minecraft/client/Minecraft", nullptr)
{
    this->Init();

    this->instance = Jvm::env->NewGlobalRef(Jvm::env->GetStaticObjectField(this->javaClass, theMinecraftFieldID));
}

Minecraft::~Minecraft() = default;

void Minecraft::Init()
{
    std::call_once(oflag, [this]
        {
            theMinecraftFieldID = Jvm::env->GetStaticFieldID(this->javaClass, "theMinecraft", "Lnet/minecraft/client/Minecraft;");

            thePlayerFieldID = Jvm::env->GetFieldID(this->javaClass, "thePlayer", "Lnet/minecraft/client/entity/EntityPlayerSP;");
            theWorldFieldID = Jvm::env->GetFieldID(this->javaClass, "theWorld", "Lnet/minecraft/client/multiplayer/WorldClient;");
            ingameGUIFieldID = Jvm::env->GetFieldID(this->javaClass, "ingameGUI", "Lnet/minecraft/client/gui/GuiIngame;");
        });
}

std::unique_ptr<EntityPlayerSP> Minecraft::GetThePlayer() const
{
    return std::make_unique<EntityPlayerSP>(Jvm::env->NewGlobalRef(Jvm::env->GetObjectField(this->instance, thePlayerFieldID)));
}

std::unique_ptr<WorldClient> Minecraft::GetTheWorld() const
{
    return std::make_unique<WorldClient>(Jvm::env->NewGlobalRef(Jvm::env->GetObjectField(this->instance, theWorldFieldID)));
}

std::unique_ptr<GuiIngame> Minecraft::GetIngameGUI() const
{
    return std::make_unique<GuiIngame>(Jvm::env->NewGlobalRef(Jvm::env->GetObjectField(this->instance, ingameGUIFieldID)));
}