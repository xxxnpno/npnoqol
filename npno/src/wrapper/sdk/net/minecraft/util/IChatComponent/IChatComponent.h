#pragma once

#include "../../src/wrapper/JavaClass/JavaClass.h"

class IChatComponent : public JavaClass
{
public:
    explicit IChatComponent(const jobject instance);
    IChatComponent(const char* name, const jobject instance);

    virtual ~IChatComponent() override;

    virtual void Init() override;

    [[nodiscard]] std::string GetFormattedText() const;
    [[nodiscard]] std::string GetUnformattedText() const;

private:
    inline static std::once_flag oflag{};

    inline static jmethodID getFormattedTextMethodID{ nullptr };
    inline static jmethodID getUnformattedTextMethodID{ nullptr };
};