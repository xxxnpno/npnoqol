#pragma once

#include "../../src/wrapper/JavaClass/JavaClass.h"

class PotionEffect final : public JavaClass
{
public:
	explicit PotionEffect(const jobject instance);

	virtual ~PotionEffect() override;

	virtual void Init() override;

	[[nodiscard]] I32 GetDuration() const;
	[[nodiscard]] I32 GetAmplifier() const;

	[[nodiscard]] std::string GetEffectName() const;

private:
	inline static std::once_flag oflag{};

	inline static jmethodID getDurationMethodID{ nullptr };
	inline static jmethodID getAmplifierMethodID{ nullptr };
	inline static jmethodID getEffectNameMethodID{ nullptr };
};