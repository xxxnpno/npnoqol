#include "Jvm.h"

#include <algorithm>

bool Jvm::Init()
{
    jsize count;
    if (JNI_GetCreatedJavaVMs(&vm, 1, &count) != JNI_OK || !count) return false;
    const jint result = vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_8);

    if (result == JNI_EDETACHED)
    {
        vm->AttachCurrentThread(reinterpret_cast<void**>(&env), nullptr);
    }

    if (vm->GetEnv(reinterpret_cast<void**>(&jvmti), JVMTI_VERSION_1_2) != JNI_OK) return false;

    std::println("[INFO] JavaVM address: {}", static_cast<const void*>(vm));
    std::println("[INFO] JNIEnv address: {}", static_cast<const void*>(env));
    std::println("[INFO] jvmtiEnv address: {}", static_cast<const void*>(jvmti));

    GetLoadedClasses();

    std::println("[INFO] JVM initialized successfully");

    return true;
}

void Jvm::ShutDown()
{
    for (const auto& [name, clazz] : classes)
    {
        if (clazz) env->DeleteGlobalRef(clazz);
    }
    classes.clear();
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
    const jclass classClass = env->FindClass("java/lang/Class");
    const jmethodID getNameID = env->GetMethodID(classClass, "getName", "()Ljava/lang/String;");

    jclass* classesPtr;
    jint amount;

    if (jvmti->GetLoadedClasses(&amount, &classesPtr) != JVMTI_ERROR_NONE)
    {
        env->DeleteLocalRef(classClass);
        return;
    }

    for (jint i = 0; i < amount; ++i)
    {
        const jclass currentClass = classesPtr[i];
        const jstring name = static_cast<jstring>(env->CallObjectMethod(currentClass, getNameID));

        if (name)
        {
            const char* const classNameCStr = env->GetStringUTFChars(name, nullptr);
            if (classNameCStr)
            {
                std::string className(classNameCStr);
                std::replace(className.begin(), className.end(), '.', '/');

                classes.emplace(std::move(className), static_cast<jclass>(env->NewGlobalRef(currentClass)));

                env->ReleaseStringUTFChars(name, classNameCStr);
            }
            env->DeleteLocalRef(name);
        }
        env->DeleteLocalRef(currentClass);
    }

    jvmti->Deallocate(reinterpret_cast<unsigned char*>(classesPtr));
    env->DeleteLocalRef(classClass);

    std::println("[INFO] Loaded {} classes", classes.size());
}