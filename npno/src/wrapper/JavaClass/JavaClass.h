#pragma once

#include "../JavaUtil/JavaUtil.h"

#include <memory>
#include <mutex>

#include <jni/jni.h>
#include <npno/Loader.hpp>

class JavaClass
{
public:
    JavaClass(const char* name, const jobject instance);

    virtual ~JavaClass();

    virtual void Init() = 0;

    [[nodiscard]] jobject GetInstance() const;

protected:
    std::once_flag oflag;

    jclass javaClass;
    jobject instance;
};