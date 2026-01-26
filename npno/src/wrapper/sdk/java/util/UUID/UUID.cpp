#include "UUID.h"

UUID_J::UUID_J(const jobject instance)
    : JavaClass("java/util/UUID", instance)
{
    this->Init();
}

UUID_J::~UUID_J() = default;

void UUID_J::Init()
{
    std::call_once(oflag, [this]
        {
            versionMethodID = Jvm::env->GetMethodID(this->javaClass, "version", "()I");
            toStringMethodID  = Jvm::env->GetMethodID(this->javaClass, "toString", "()Ljava/lang/String;");
        });
}

jint UUID_J::Version() const
{
    return Jvm::env->CallIntMethod(this->instance, versionMethodID);
}

jstring UUID_J::ToString() const
{
    return static_cast<jstring>(Jvm::env->CallObjectMethod(this->instance, toStringMethodID));
}