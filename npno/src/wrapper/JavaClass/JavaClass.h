#pragma once

#include "../JavaUtil/JavaUtil.h"

#include <memory>
#include <mutex>

#include <jni/jni.h>
#include <npno/Loader.hpp>

class JavaClass
{
public:
    JavaClass(const char* const name, const jobject instance);

    virtual ~JavaClass();

    virtual void Init() = 0;

    [[nodiscard]] jobject GetInstance() const;

protected:
    jclass javaClass;
    jobject instance;
};