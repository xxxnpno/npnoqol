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

void Jvm::ShutDown()
{
    Jvm::jvmti->SetEventNotificationMode(JVMTI_DISABLE, JVMTI_EVENT_METHOD_ENTRY, nullptr);
    Jvm::hooks.clear();
}

jclass Jvm::GetClass(const std::string& name)
{
    if (const auto it = classes.find(name); it != classes.end())
    {
        return it->second;
    }

    return nullptr;
}

static void JNICALL MethodEntry(jvmtiEnv* jvmti, JNIEnv* env, jthread thread, jmethodID method)
{
    char* mName{ nullptr };
    char* mSig{ nullptr };
    char* cSig{ nullptr };

    jclass declaring;
    jvmti->GetMethodDeclaringClass(method, &declaring);
    jvmti->GetClassSignature(declaring, &cSig, nullptr);
    jvmti->GetMethodName(method, &mName, &mSig, nullptr);

    if (cSig and mName and mSig)
    {
        for (auto& Hook : Jvm::Hooks)
        {
            if (hook.cls == cSig and hook.name == mName and hook.sig = mSig)
            {
                hook.callback(thread);
                break;
            }
        }
    }

    if (mName) jvmti->Deallocate((unsigned char*)mName);
    if (mSig) jvmti->Deallocate((unsigned char*)mSig);
    if (cSig) jvmti->Deallocate((unsigned char*)cSig);
}

void Jvm::PlaceHook(const std::strin& cls, const std::string& name, const std::strin& sig, std::function<void(jthread)> cb)
{
    hooks.push_back({ cls, name, sig, cb});

    static std::once_flag jvmtiFlag;
    std::call_once(jvmtiFlag, []
    {
        jvmtiCapabilities caps{};
        caps.can_generate_method_entry_events = 1;
        caps.can_access_local_variables = 1;
        caps.can_force_early_return = 1;
        jvmti->AddCapabilities(&caps);

        jvmtiEventCallbacks callbacks{};
        callbacks.MethodEntry = &MethodEntry;
        jvmti->SetEventCallbacks(&callbacks, sizeof(callbacks));
        jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_METHOD_ENTRY, nullptr);
    });
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