#pragma once

#include "../../src/wrapper/JavaClass/JavaClass.h"

#include "../../src/wrapper/sdk/net/minecraft/world/storage/WorldInfo/WorldInfo.h"
#include "../../src/wrapper/sdk/net/minecraft/entity/player/EntityPlayer/EntityPlayer.h"

class World : public JavaClass
{
public:
	explicit World(const jobject instance);

	World(const char* name, const jobject instance);

	virtual ~World() override;

	virtual void Init() override;

	[[nodiscard]] virtual std::unique_ptr<WorldInfo> GetWorldInfo() const final;

	[[nodiscard]] virtual std::vector<std::unique_ptr<EntityPlayer>> GetPlayerEntities() const final;

private:
	inline static std::once_flag oflag{};

	inline static jfieldID worldInfoFieldID{ nullptr };
	inline static jfieldID playerEntitiesFieldID{ nullptr };
};