#include "PropertyMap.h"

#include "../../src/wrapper/sdk/java/util/Collection/Collection.h"

PropertyMap::PropertyMap(const jobject instance)
    : JavaClass("com/mojang/authlib/properties/PropertyMap", instance)
{
    this->Init();
}

PropertyMap::~PropertyMap() = default;

void PropertyMap::Init()
{
    std::call_once(oflag, [this]
        {
            valuesMethodID = Jvm::env->GetMethodID(this->javaClass, "values", "()Ljava/util/Collection;");
        });
}

std::vector<std::unique_ptr<Property>> PropertyMap::GetValues() const
{
    std::vector<std::unique_ptr<Property>> properties;

    std::unique_ptr<Collection> collection = std::make_unique<Collection>(Jvm::env->CallObjectMethod(this->instance, valuesMethodID));
    const jobjectArray array = collection->ToArray();

    for (jint i = 0; i < collection->Size(); ++i)
    {
        properties.push_back(std::make_unique<Property>(Jvm::env->NewGlobalRef(Jvm::env->GetObjectArrayElement(array, i))));
    }

    Jvm::env->DeleteGlobalRef(array);

    return properties;
}
