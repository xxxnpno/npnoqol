#pragma once

#include "../../src/wrapper/JavaClass/JavaClass.h"

class UUID_J final : public JavaClass
{
public:
    explicit UUID_J(const jobject instance);

    virtual ~UUID_J() override;

    virtual void Init() override;

    [[nodiscard]] jint Version() const;

    [[nodiscard]] jstring ToString() const;  

private:
    inline static std::once_flag oflag{};

    inline static jmethodID versionMethodID{ nullptr };
    inline static jmethodID toStringMethodID{ nullptr };
};