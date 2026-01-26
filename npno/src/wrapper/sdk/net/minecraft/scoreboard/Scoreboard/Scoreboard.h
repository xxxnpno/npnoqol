#pragma once

#include "../../src/wrapper/JavaClass/JavaClass.h"

#include "../../src/wrapper/sdk/net/minecraft/scoreboard/Score/Score.h"
#include "../../src/wrapper/sdk/net/minecraft/scoreboard/ScoreObjective/ScoreObjective.h"
#include "../../src/wrapper/sdk/net/minecraft/scoreboard/ScorePlayerTeam/ScorePlayerTeam.h"

class Scoreboard final : public JavaClass
{
public:
	explicit Scoreboard(const jobject instance);

	virtual ~Scoreboard() override;

	enum class DisplaySlot : I8
	{
		LIST,
		SIDEBAR,
		BELOW_NAME
	};

	virtual void Init() override;

	[[nodiscard]] bool AddPlayerToTeam(const std::string& playerName, const std::string& teamName) const;

	[[nodiscard]] std::unique_ptr<ScorePlayerTeam> GetTeam(const std::string& teamName) const;
	[[nodiscard]] std::unique_ptr<ScorePlayerTeam> GetPlayersTeam(const std::string& playerName) const;
	[[nodiscard]] std::unique_ptr<ScorePlayerTeam> CreateTeam(const std::string& teamName) const;

	[[nodiscard]] std::unique_ptr<ScoreObjective> GetObjectiveInDisplaySlot(const DisplaySlot slot) const;

	[[nodiscard]] std::vector<std::unique_ptr<ScorePlayerTeam>> GetTeams() const;

	[[nodiscard]] std::vector<std::unique_ptr<Score>> GetSortedScores(const std::unique_ptr<ScoreObjective>& objective) const;

	void RemoveTeam(const std::unique_ptr<ScorePlayerTeam>& team) const;
	void RemovePlayerFromTeam(const std::string& playerName, const std::unique_ptr<ScorePlayerTeam>& team) const;
	void SetObjectiveInDisplaySlot(const DisplaySlot slot, const std::unique_ptr<ScoreObjective>& objective) const;

private:
	inline static std::once_flag oflag{};

	inline static jmethodID addPlayerToTeamMethodID{ nullptr };
	inline static jmethodID getTeamMethodID{ nullptr };
	inline static jmethodID getPlayersTeamMethodID{ nullptr };
	inline static jmethodID createTeamMethodID{ nullptr };
	inline static jmethodID getObjectiveInDisplaySlotMethodID{ nullptr };
	inline static jmethodID getTeamsMethodID{ nullptr };
	inline static jmethodID getSortedScoresMethodID{ nullptr };
	inline static jmethodID removeTeamMethodID{ nullptr };
	inline static jmethodID removePlayerFromTeamMethodID{ nullptr };
	inline static jmethodID setObjectiveInDisplaySlotMethodID{ nullptr };
};