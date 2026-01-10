#pragma once

#include "../../src/wrapper/JavaClass/JavaClass.h"

#include "../../src/wrapper/sdk/net/minecraft/util/ChatComponentText/ChatComponentText.h"

class Entity : public JavaClass
{
public:
	explicit Entity(const jobject instance);
	Entity(const char* name, const jobject instance);

	virtual ~Entity() override;

	virtual void Init() override;

	[[nodiscard]] virtual bool IsInvisible() const final;

	[[nodiscard]] virtual std::string GetName() const final;

	virtual void AddChatMessage(const std::unique_ptr<IChatComponent>& chatComponent) const final;

private:
	inline static jmethodID isInvisibleMethodID{ nullptr };
	inline static jmethodID getNameMethodID{ nullptr };
	inline static jmethodID addChatMessageMethodID{ nullptr };
};