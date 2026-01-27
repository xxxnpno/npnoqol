#include "ChatLine.h"

ChatLine::ChatLine(const jobject instance)
    : JavaClass("net/minecraft/client/gui/ChatLine", instance)
{
    this->Init();
}

ChatLine::~ChatLine() = default;

void ChatLine::Init()
{
    std::call_once(oflag, [this]
        {
			chatLineIDFieldID = Jvm::env->GetFieldID(this->javaClass, "chatLineID", "I");
            lineStringFieldID = Jvm::env->GetFieldID(this->javaClass, "lineString", "Lnet/minecraft/util/IChatComponent;");
        });
}

I32 ChatLine::GetChatLineID() const
{
    return Jvm::env->GetIntField(this->instance, chatLineIDFieldID);
}

std::unique_ptr<IChatComponent> ChatLine::GetLineString() const
{
    return std::make_unique<IChatComponent>(Jvm::env->NewGlobalRef(Jvm::env->GetObjectField(this->instance, lineStringFieldID)));
}

void ChatLine::SetLineString(const std::unique_ptr<IChatComponent>& newLine)
{
    Jvm::env->SetObjectField(this->instance, lineStringFieldID, newLine->GetInstance());
}