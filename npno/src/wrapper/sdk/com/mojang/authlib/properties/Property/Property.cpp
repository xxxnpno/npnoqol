#include "Property.h"

Property::Property(const jobject instance)
    : JavaClass("com/mojang/authlib/properties/Property", instance)
{
    this->Init();
}

Property::~Property() = default;

void Property::Init()
{
    std::call_once(oflag, [this]
        {
            getValueMethodID = Jvm::env->GetMethodID(this->javaClass, "getValue", "()Ljava/lang/String;");
            getNameMethodID = Jvm::env->GetMethodID(this->javaClass, "getName", "()Ljava/lang/String;");
        });
}

jstring Property::GetValue() const
{
    return static_cast<jstring>(Jvm::env->CallObjectMethod(this->instance, getValueMethodID));
}

jstring Property::GetName() const
{
    return static_cast<jstring>(Jvm::env->CallObjectMethod(this->instance, getNameMethodID));
}
