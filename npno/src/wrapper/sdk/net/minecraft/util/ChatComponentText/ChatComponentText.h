#pragma once

#include "../../src/wrapper/sdk/net/minecraft/util/IChatComponent/IChatComponent.h"

class ChatComponentText final : public IChatComponent
{
public:
	explicit ChatComponentText(const jobject instance);
	explicit ChatComponentText(const std::string& text);

	virtual ~ChatComponentText() override;

	virtual void Init() override;

private:
	inline static std::once_flag oflag{};

	inline static jmethodID constructorMethodID{ nullptr };
};