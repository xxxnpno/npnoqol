#pragma once

#include "../../src/wrapper/JavaClass/JavaClass.h"

class ScoreObjective final : public JavaClass
{
public:
	explicit ScoreObjective(const jobject instance);

	virtual ~ScoreObjective() override;

	virtual void Init() override;

	[[nodiscard]] std::string GetName() const;
	[[nodiscard]] std::string GetDisplayName() const;

	void SetDisplayName(const std::string& name) const;

private:
	inline static std::once_flag oflag{};

	inline static jmethodID getNameMethodID{ nullptr };
	inline static jmethodID getDisplayNameMethodID{ nullptr };
	inline static jmethodID setDisplayNameMethodID{ nullptr };
};