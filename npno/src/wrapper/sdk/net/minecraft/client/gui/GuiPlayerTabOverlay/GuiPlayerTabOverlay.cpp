#include "GuiPlayerTabOverlay.h"

GuiPlayerTabOverlay::GuiPlayerTabOverlay(const jobject instance)
    : JavaClass("net/minecraft/client/gui/GuiPlayerTabOverlay", instance)
{
    this->Init();
}

GuiPlayerTabOverlay::~GuiPlayerTabOverlay() = default;

void GuiPlayerTabOverlay::Init() 
{
    std::call_once(oflag, [this] 
        {
        getPlayerNameMethodID = Jvm::env->GetMethodID(this->javaClass,"getPlayerName","(Lnet/minecraft/client/network/NetworkPlayerInfo;)Ljava/lang/String;");
        });
}

std::string GuiPlayerTabOverlay::GetPlayerName(const std::unique_ptr<NetworkPlayerInfo>& playerInfo) const 
{
    return JavaUtil::JStringToString(static_cast<jstring>(Jvm::env->CallObjectMethod(this->instance, getPlayerNameMethodID, playerInfo->GetInstance())));
}