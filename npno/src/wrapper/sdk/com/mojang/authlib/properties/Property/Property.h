#pragma once

#include "../../src/wrapper/JavaClass/JavaClass.h"

class Property final : public JavaClass
{
public:
    explicit Property(const jobject instance);

    virtual ~Property() override;

    virtual void Init() override;

    [[nodiscard]] jstring GetValue() const;
    [[nodiscard]] jstring GetName() const;

private:
    inline static std::once_flag oflag{};

    inline static jmethodID getValueMethodID{ nullptr };
    inline static jmethodID getNameMethodID{ nullptr };
};
