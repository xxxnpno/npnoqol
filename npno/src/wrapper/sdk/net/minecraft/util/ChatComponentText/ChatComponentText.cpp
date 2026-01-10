#include "ChatComponentText.h"

ChatComponentText::ChatComponentText(const jobject instance)
    : IChatComponent("net/minecraft/util/ChatComponentText", instance)
{
    this->Init();
}

ChatComponentText::ChatComponentText(const std::string& text)
    : IChatComponent("net/minecraft/util/ChatComponentText", nullptr)
{
    this->Init();

    this->instance = Jvm::env->NewGlobalRef(Jvm::env->NewObject(this->javaClass, constructorMethodID, JavaUtil::StringToJString(JavaUtil::FixString(text))));
}

ChatComponentText::~ChatComponentText() = default;

void ChatComponentText::Init()
{
    std::call_once(this->chatComponentTextOFlag, [this]
        {
            constructorMethodID = Jvm::env->GetMethodID(this->javaClass, "<init>", "(Ljava/lang/String;)V");
        });
}
