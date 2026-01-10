#include "Collection.h"

Collection::Collection(const jobject instance) 
    : JavaClass("java/util/Collection", instance) 
{
    this->Init();
}

Collection::Collection(const char* name, const jobject instance) 
    : JavaClass(name, instance) 
{
    this->Init();
}

Collection::~Collection() = default;

void Collection::Init() 
{
    std::call_once(this->oflag, [this] 
        {
        sizeMethodID = Jvm::env->GetMethodID(this->javaClass, "size", "()I");
        toArrayMethodID = Jvm::env->GetMethodID(this->javaClass, "toArray", "()[Ljava/lang/Object;");
        });
}

jint Collection::Size() const 
{
    return Jvm::env->CallIntMethod(this->instance, sizeMethodID);
}

jobjectArray Collection::ToArray() const 
{
    return static_cast<jobjectArray>(Jvm::env->NewGlobalRef(Jvm::env->CallObjectMethod(this->instance, toArrayMethodID)));
}