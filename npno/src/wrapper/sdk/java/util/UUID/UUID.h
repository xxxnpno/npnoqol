#pragma once

#include "../../src/wrapper/JavaClass/JavaClass.h"

class UUID_J final : public JavaClass
{
public:
    explicit UUID_J(const jobject instance);

    virtual ~UUID_J() override;

    virtual void Init() override;

    [[nodiscard]] virtual jint Version() const final;

private:
    inline static std::once_flag oflag{};

    inline static jmethodID versionMethodID{ nullptr };
};