#include "GuiNewChat.h"

#include "../../src/wrapper/sdk/java/util/List/List.h"

GuiNewChat::GuiNewChat(const jobject instance)
	: JavaClass("net/minecraft/client/gui/GuiNewChat", instance)
{
	this->Init();
}

GuiNewChat::~GuiNewChat() = default;

void GuiNewChat::Init()
{
	std::call_once(oflag, [this]
		{
			chatLinesFieldID = Jvm::env->GetFieldID(this->javaClass, "chatLines", "Ljava/util/List;");

			refreshChatMethodID = Jvm::env->GetMethodID(this->javaClass, "refreshChat", "()V");
			deleteChatLineMethodID = Jvm::env->GetMethodID(this->javaClass, "deleteChatLine", "(I)V");
		});
}

std::vector<std::unique_ptr<ChatLine>> GuiNewChat::GetChatLines() const
{
	std::vector<std::unique_ptr<ChatLine>> chat;

	const jobject chatLineLocal = Jvm::env->GetObjectField(this->instance, chatLinesFieldID);

	const std::unique_ptr<List> chatLines = std::make_unique<List>(chatLineLocal);

	for (jint i = 0; i < chatLines->Size(); ++i)
	{
		const jobject lineLocal = chatLines->Get(i);
		chat.emplace_back(std::make_unique<ChatLine>(lineLocal));
		Jvm::env->DeleteLocalRef(lineLocal);
	}

	Jvm::env->DeleteLocalRef(chatLineLocal);
	return chat;
}

void GuiNewChat::RefreshChat() const
{
	Jvm::env->CallVoidMethod(this->instance, refreshChatMethodID);
}

void GuiNewChat::DeleteChatLine(const I32 id) const
{
	Jvm::env->CallVoidMethod(this->instance, deleteChatLineMethodID, id);
}