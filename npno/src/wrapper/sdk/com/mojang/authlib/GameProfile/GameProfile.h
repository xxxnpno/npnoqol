#pragma once

#include "../../src/wrapper/JavaClass/JavaClass.h"

class GameProfile final : public JavaClass
{
public:
	explicit GameProfile(const jobject instance);

	virtual ~GameProfile() override;

	virtual void Init() override;

	[[nodiscard]] std::string GetName() const;

private:
	inline static jmethodID getNameMethodID{ nullptr };
};