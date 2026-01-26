#pragma once

#include "../../src/wrapper/JavaClass/JavaClass.h"

#include "../../src/wrapper/sdk/net/minecraft/client/network/NetworkPlayerInfo/NetworkPlayerInfo.h"

class NetHandlerPlayClient final : public JavaClass
{
public:
	explicit NetHandlerPlayClient(const jobject instance);

	virtual ~NetHandlerPlayClient() override;

	virtual void Init() override;

	[[nodiscard]] std::vector<std::unique_ptr<NetworkPlayerInfo>> GetPlayerInfoMap() const;

private:
	inline static std::once_flag oflag{};

	inline static jmethodID getPlayerInfoMapMethodID{ nullptr };
};