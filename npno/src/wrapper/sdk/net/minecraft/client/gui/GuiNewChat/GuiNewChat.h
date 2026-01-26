#pragma once

#include "../../src/wrapper/JavaClass/JavaClass.h"

#include "../../src/wrapper/sdk/net/minecraft/client/gui/ChatLine/ChatLine.h"

class GuiNewChat final : public JavaClass
{
public:
	explicit GuiNewChat(const jobject instance);

	virtual ~GuiNewChat() override;

	virtual void Init() override;

	[[nodiscard]] std::vector<std::unique_ptr<ChatLine>> GetChatLines() const;

	void RefreshChat() const;
	void DeleteChatLine(const I32 id) const;

private:
	inline static std::once_flag oflag{};

	inline static jfieldID chatLinesFieldID{ nullptr };

	inline static jmethodID refreshChatMethodID{ nullptr };
	inline static jmethodID deleteChatLineMethodID{ nullptr };
};