#pragma once

#include <print>
#include <string>
#include <unordered_map>

#include <jni/jni.h>
#include <jni/jvmti.h>

class Jvm final
{
public:
    static bool Init();
    static void ShutDown();

    static jclass GetClass(const std::string& name);

    inline static JavaVM* vm{ nullptr };
    inline static JNIEnv* env{ nullptr };
    inline static jvmtiEnv* jvmti{ nullptr };

    struct Hook 
    {
        std::string cls;
        std::string name;
        std::string sig;
        std::function<void(jthread)> callback;
    };

    static void PlaceHook(const std::string& cls, const std::string& name, const std::string& sig, std::function<void(jthread)> cb);

    inline static std::vector<Hook> hooks;

private:
    static void GetLoadedClasses();

    inline static std::unordered_map<std::string, jclass> classes;
};