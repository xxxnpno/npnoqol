#include "IChatComponent.h"

IChatComponent::IChatComponent(const jobject instance)
    : JavaClass("net/minecraft/util/IChatComponent", instance)
{
    this->Init();
}

IChatComponent::IChatComponent(const char* name, const jobject instance)
    : JavaClass(name, instance)
{
    this->Init();
}

IChatComponent::~IChatComponent() = default;

void IChatComponent::Init()
{
    std::call_once(this->oflag, [this]
        {
            getFormattedTextMethodID = Jvm::env->GetMethodID(this->javaClass, "getFormattedText", "()Ljava/lang/String;");
            getUnformattedTextMethodID = Jvm::env->GetMethodID(this->javaClass, "getUnformattedText", "()Ljava/lang/String;");
        });
}

std::string IChatComponent::GetFormattedText() const
{
    return JavaUtil::JStringToString(static_cast<jstring>(Jvm::env->CallObjectMethod(this->instance, getFormattedTextMethodID)));
}

std::string IChatComponent::GetUnformattedText() const
{
    return JavaUtil::JStringToString(static_cast<jstring>(Jvm::env->CallObjectMethod(this->instance, getUnformattedTextMethodID)));
}