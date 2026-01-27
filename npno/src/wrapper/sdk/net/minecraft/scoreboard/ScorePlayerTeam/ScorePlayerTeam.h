#pragma once

#include "../../src/wrapper/JavaClass/JavaClass.h"

class ScorePlayerTeam final : public JavaClass
{
public:
	explicit ScorePlayerTeam(const jobject instance);

	virtual ~ScorePlayerTeam() override;

	virtual void Init() override;

	[[nodiscard]] std::string GetNamePrefix() const;
	[[nodiscard]] std::string GetNameSuffix() const;
	[[nodiscard]] std::string GetTeamName() const;

	[[nodiscard]] std::vector<std::string> GetMembershipCollection() const;

	void SetNamePrefix(const std::string& prefix) const;
	void SetNameSuffix(const std::string& suffix) const;

private:
	inline static std::once_flag oflag{};

	inline static jmethodID getNamePrefixMethodID{ nullptr };
	inline static jmethodID getNameSuffixMethodID{ nullptr };
	inline static jmethodID getTeamNameMethodID{ nullptr };
	inline static jmethodID getMembershipCollectionMethodID{ nullptr };
	inline static jmethodID setNamePrefixMethodID{ nullptr };
	inline static jmethodID setNameSuffixMethodID{ nullptr };
};