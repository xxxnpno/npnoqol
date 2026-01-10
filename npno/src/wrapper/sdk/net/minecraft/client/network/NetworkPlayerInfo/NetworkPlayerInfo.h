#pragma once

#include "../../src/wrapper/JavaClass/JavaClass.h"

#include "../../src/wrapper/sdk/com/mojang/authlib/GameProfile/GameProfile.h"
#include "../../src/wrapper/sdk/net/minecraft/util/IChatComponent/IChatComponent.h"

class NetworkPlayerInfo final : public JavaClass
{
public:
	explicit NetworkPlayerInfo(const jobject instance);

	virtual ~NetworkPlayerInfo() override;

	virtual void Init() override;

	[[nodiscard]] std::unique_ptr<GameProfile> GetGameProfile() const;
	[[nodiscard]] std::unique_ptr<IChatComponent> GetDisplayName() const;

	void SetDisplayName(const std::unique_ptr<IChatComponent>& newName);

private:
	inline static jmethodID getGameProfileMethodID{ nullptr };
	inline static jmethodID getDisplayNameMethodID{ nullptr };
	inline static jmethodID setDisplayNameMethodID{ nullptr };
};