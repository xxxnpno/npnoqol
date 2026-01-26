#pragma once

#include "../../src/wrapper/sdk/net/minecraft/entity/player/EntityPlayer/EntityPlayer.h"

#include "../../src/wrapper/sdk/net/minecraft/client/network/NetHandlerPlayClient/NetHandlerPlayClient.h"

class EntityPlayerSP final : public EntityPlayer
{
public:
	explicit EntityPlayerSP(const jobject instance);

	virtual ~EntityPlayerSP() override;

	virtual void Init() override;

	std::unique_ptr<NetHandlerPlayClient> GetSendQueue() const;

	void SendChatMessage(const std::string& message) const;

private:
	inline static std::once_flag oflag{};

	inline static jfieldID sendQueueFieldID{ nullptr };

	inline static jmethodID sendChatMessageMethodID{ nullptr };
};