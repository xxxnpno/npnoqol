#pragma once

#include "../../src/wrapper/JavaClass/JavaClass.h"

#include "../../src/wrapper/sdk/net/minecraft/scoreboard/ScorePlayerTeam/ScorePlayerTeam.h"

class Scoreboard final : public JavaClass
{
public:
	explicit Scoreboard(const jobject instance);

	virtual ~Scoreboard() override;

	virtual void Init() override;

	[[nodiscard]] bool AddPlayerToTeam(const std::string& playerName, const std::string& teamName) const;

	[[nodiscard]] std::unique_ptr<ScorePlayerTeam> GetTeam(const std::string& teamName) const;
	[[nodiscard]] std::unique_ptr<ScorePlayerTeam> CreateTeam(const std::string& teamName) const;

	[[nodiscard]] std::vector<std::unique_ptr<ScorePlayerTeam>> GetTeams() const;

	void RemoveTeam(const std::unique_ptr<ScorePlayerTeam>& team) const;
	void RemovePlayerFromTeam(const std::string& playerName) const;

private:
	inline static jmethodID addPlayerToTeamMethodID{ nullptr };
	inline static jmethodID getTeamMethodID{ nullptr };
	inline static jmethodID createTeamMethodID{ nullptr };
	inline static jmethodID getTeamsMethodID{ nullptr };
	inline static jmethodID removeTeamMethodID{ nullptr };
	inline static jmethodID removePlayerFromTeamMethodID{ nullptr };
};