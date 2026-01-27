#pragma once

#include "../../src/wrapper/JavaClass/JavaClass.h"

class Score final : public JavaClass
{
public:
	explicit Score(const jobject instance);

	virtual ~Score() override;

	virtual void Init() override;

	[[nodiscard]] I32 GetScorePoints() const;

	[[nodiscard]] std::string GetPlayerName() const;

private:
	inline static std::once_flag oflag{};

	inline static jmethodID getScorePointsMethodID{ nullptr };
	inline static jmethodID getPlayerNameMethodID{ nullptr };
};