#include "../../src/wrapper/JavaClass/JavaClass.h"

#include "../../src/wrapper/sdk/net/minecraft/client/network/NetworkPlayerInfo/NetworkPlayerInfo.h"

class GuiPlayerTabOverlay final : public JavaClass 
{
public:
	explicit GuiPlayerTabOverlay(const jobject instance);

	virtual ~GuiPlayerTabOverlay() override;

	virtual void Init() override;

	[[nodiscard]] std::string GetPlayerName(const std::unique_ptr<NetworkPlayerInfo>& playerInfo) const;

private:
	inline static jmethodID getPlayerNameMethodID{ nullptr };
};