#pragma once

#include <string>

#include "../Jvm/Jvm.h"

class JavaUtil final
{
public:
    static std::string FixString(const std::string& string);
    static std::string JStringToString(const jstring javaString);

    static jstring StringToJString(const std::string& string);
};