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
        });
}

jint UUID_J::Version() const
{
    return Jvm::env->CallIntMethod(this->instance, versionMethodID);
}