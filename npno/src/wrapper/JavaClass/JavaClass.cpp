#include "JavaClass.h"

#include <print>

JavaClass::JavaClass(const char* const name, const jobject instance)
    : javaClass{ nullptr }
    , instance{ instance ? Jvm::env->NewGlobalRef(instance) : nullptr }
{
    const jclass tempClass = Jvm::GetClass(name);
    if (tempClass)
    {
        this->javaClass = static_cast<jclass>(Jvm::env->NewGlobalRef(tempClass));
    }
}

JavaClass::~JavaClass()
{
    if (this->instance) Jvm::env->DeleteGlobalRef(this->instance);
    if (this->javaClass) Jvm::env->DeleteGlobalRef(this->javaClass);
}

jobject JavaClass::GetInstance() const
{
    return this->instance;
}