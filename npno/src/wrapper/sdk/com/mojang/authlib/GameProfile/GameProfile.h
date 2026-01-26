#pragma once

#include "../../src/wrapper/JavaClass/JavaClass.h"

#include "../../src/wrapper/sdk/com/mojang/authlib/properties/PropertyMap/PropertyMap.h"

class GameProfile final : public JavaClass
{
public:
	explicit GameProfile(const jobject instance);

	virtual ~GameProfile() override;

	virtual void Init() override;

	[[nodiscard]] std::string GetName() const;

	[[nodiscard]] std::unique_ptr<PropertyMap> GetProperties() const;

private:
	inline static std::once_flag oflag{};

	inline static jmethodID getNameMethodID{ nullptr };
	inline static jmethodID getPropertiesMethodID{ nullptr };
};
