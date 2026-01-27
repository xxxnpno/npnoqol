#include "GuiIngame.h"

GuiIngame::GuiIngame(const jobject instance)
	: JavaClass("net/minecraft/client/gui/GuiIngame", instance)
{
	this->Init();
}

GuiIngame::~GuiIngame() = default;

void GuiIngame::Init()
{
	std::call_once(oflag, [this]
		{
			persistantChatGUIFieldID = Jvm::env->GetFieldID(this->javaClass, "persistantChatGUI", "Lnet/minecraft/client/gui/GuiNewChat;");
            overlayPlayerListFieldID = Jvm::env->GetFieldID(this->javaClass, "overlayPlayerList", "Lnet/minecraft/client/gui/GuiPlayerTabOverlay;");
		});
}

std::unique_ptr<GuiNewChat> GuiIngame::GetPersistantChatGUI() const
{
	return std::make_unique<GuiNewChat>(Jvm::env->NewGlobalRef(Jvm::env->GetObjectField(this->instance, persistantChatGUIFieldID)));
}

std::unique_ptr<GuiPlayerTabOverlay> GuiIngame::GetOverlayPlayerList() const 
{
    return std::make_unique<GuiPlayerTabOverlay>(Jvm::env->NewGlobalRef(Jvm::env->GetObjectField(this->instance, overlayPlayerListFieldID)));
}