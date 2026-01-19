#include "JavaUtil.h"

std::string JavaUtil::FixString(const std::string& string)
{
    size_t count = 0;

    for (const char c : string)
    {
        if (c == '§')
        {
            ++count;
        }
    }

    return string + std::string(count, ' ');
}

std::string JavaUtil::JStringToString(const jstring javaString)
{
    if (!javaString) return "";
    
    const char* chars = Jvm::env->GetStringUTFChars(javaString, nullptr);
    std::string result(chars);

    Jvm::env->ReleaseStringUTFChars(javaString, chars);

    return result;
}

jstring JavaUtil::StringToJString(const std::string& string)
{
    return Jvm::env->NewStringUTF(string.c_str());
}