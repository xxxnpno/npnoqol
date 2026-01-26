#pragma once

#include "../../src/wrapper/JavaClass/JavaClass.h"

#include "../../src/wrapper/sdk/net/minecraft/util/IChatComponent/IChatComponent.h"

class ChatLine final : public JavaClass
{
public:
    explicit ChatLine(const jobject instance);

    virtual ~ChatLine() override;

    virtual void Init() override;

	[[nodiscard]] I32 GetChatLineID() const;

    [[nodiscard]] std::unique_ptr<IChatComponent> GetLineString() const;

    void SetLineString(const std::unique_ptr<IChatComponent>& newLine);

private:
    inline static std::once_flag oflag{};

	inline static jfieldID chatLineIDFieldID{ nullptr };
    inline static jfieldID lineStringFieldID{ nullptr };
};