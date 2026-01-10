#include "JavaClass.h"

#include <print>

JavaClass::JavaClass(const char* name, const jobject instance)
    : javaClass{ static_cast<jclass>(Jvm::env->NewGlobalRef(Jvm::GetClass(name))) }
    , instance{ instance ? Jvm::env->NewGlobalRef(instance) : nullptr }
{
    
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