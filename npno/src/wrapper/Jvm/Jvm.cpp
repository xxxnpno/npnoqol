#include "Jvm.h"

#include <algorithm>

bool Jvm::Init()
{
    jsize count;
    if (JNI_GetCreatedJavaVMs(&vm, 1, &count) != JNI_OK || !count) return false;
    jint result = vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_8);

    if (result == JNI_EDETACHED)
    {
        result = vm->AttachCurrentThread(reinterpret_cast<void**>(&env), nullptr);
    }

    if (vm->GetEnv(reinterpret_cast<void**>(&jvmti), JVMTI_VERSION_1_2) != JNI_OK) return false;

    std::println("[INFO] JavaVM address: {}", static_cast<void*>(vm)); 
    std::println("[INFO] JNIEnv address: {}", static_cast<void*>(env)); 
    std::println("[INFO] jvmtiEnv address: {}", static_cast<void*>(jvmti));

    GetLoadedClasses();

    std::println("[INFO] JVM initialized successfully");

    return true;
}

jclass Jvm::GetClass(const std::string& name)
{
    if (const auto it = classes.find(name); it != classes.end())
    {
        return it->second;
    }

    return nullptr;
}

void Jvm::GetLoadedClasses()
{
    const jmethodID getNameID = env->GetMethodID(env->FindClass("java/lang/Class"), "getName", "()Ljava/lang/String;");

    jclass* classesPtr;
    jint amount;

    jvmti->GetLoadedClasses(&amount, &classesPtr);

    for (jint i = 0; i < amount; ++i)
    {
        const jstring name = static_cast<jstring>(env->CallObjectMethod(classesPtr[i], getNameID));
        if (name)
        {
            const char* classNameCStr = env->GetStringUTFChars(name, 0);
            if (classNameCStr)
            {
                std::string className(classNameCStr);

                std::replace(className.begin(), className.end(), '.', '/');

                classes.emplace(std::make_pair(className, classesPtr[i]));
                env->ReleaseStringUTFChars(name, classNameCStr);
            }
        }
    }

    std::println("[INFO] Loaded {} classes", classes.size());
}