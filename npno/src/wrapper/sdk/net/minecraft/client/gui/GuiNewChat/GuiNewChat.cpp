#include "GuiNewChat.h"

GuiNewChat::GuiNewChat(const jobject instance)
	: JavaClass("net/minecraft/client/gui/GuiNewChat", instance)
{
	this->Init();
}

GuiNewChat::~GuiNewChat() = default;

void GuiNewChat::Init()
{
	std::call_once(this->oflag, [this]
		{

		});
}