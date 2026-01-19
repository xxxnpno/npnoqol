#include "Scoreboard.h"

#include "../../src/wrapper/sdk/java/util/Collection/Collection.h"

Scoreboard::Scoreboard(const jobject instance)
    : JavaClass("net/minecraft/scoreboard/Scoreboard", instance)
{
    this->Init();
}

Scoreboard::~Scoreboard() = default;

void Scoreboard::Init() 
{
    std::call_once(this->oflag, [this]
        {
            addPlayerToTeamMethodID = Jvm::env->GetMethodID(this->javaClass, "addPlayerToTeam", "(Ljava/lang/String;Ljava/lang/String;)Z");
            getTeamMethodID = Jvm::env->GetMethodID(this->javaClass, "getTeam", "(Ljava/lang/String;)Lnet/minecraft/scoreboard/ScorePlayerTeam;");
            getPlayersTeamMethodID = Jvm::env->GetMethodID(this->javaClass, "getPlayersTeam", "(Ljava/lang/String;)Lnet/minecraft/scoreboard/ScorePlayerTeam;");
            createTeamMethodID = Jvm::env->GetMethodID(this->javaClass, "createTeam", "(Ljava/lang/String;)Lnet/minecraft/scoreboard/ScorePlayerTeam;");
			getTeamsMethodID = Jvm::env->GetMethodID(this->javaClass, "getTeams", "()Ljava/util/Collection;");
            removeTeamMethodID = Jvm::env->GetMethodID(this->javaClass, "removeTeam", "(Lnet/minecraft/scoreboard/ScorePlayerTeam;)V");
            removePlayerFromTeamMethodID = Jvm::env->GetMethodID(this->javaClass, "removePlayerFromTeam", "(Ljava/lang/String;Lnet/minecraft/scoreboard/ScorePlayerTeam;)V");
        });
}

bool Scoreboard::AddPlayerToTeam(const std::string& playerName, const std::string& teamName) const 
{
    return Jvm::env->CallBooleanMethod(this->instance, addPlayerToTeamMethodID, JavaUtil::StringToJString(playerName), JavaUtil::StringToJString(teamName));
}

std::unique_ptr<ScorePlayerTeam> Scoreboard::GetTeam(const std::string& teamName) const 
{
    return std::make_unique<ScorePlayerTeam>(Jvm::env->NewGlobalRef(Jvm::env->CallObjectMethod(this->instance, getTeamMethodID, JavaUtil::StringToJString(teamName))));
}

std::unique_ptr<ScorePlayerTeam> Scoreboard::GetPlayersTeam(const std::string& playerName) const
{
    return std::make_unique<ScorePlayerTeam>(Jvm::env->NewGlobalRef(Jvm::env->CallObjectMethod(this->instance, getPlayersTeamMethodID, JavaUtil::StringToJString(playerName))));
}

std::unique_ptr<ScorePlayerTeam> Scoreboard::CreateTeam(const std::string& teamName) const 
{
    return std::make_unique<ScorePlayerTeam>(Jvm::env->NewGlobalRef(Jvm::env->CallObjectMethod(this->instance, createTeamMethodID, JavaUtil::StringToJString(teamName))));
}

std::vector<std::unique_ptr<ScorePlayerTeam>> Scoreboard::GetTeams() const
{
    std::vector<std::unique_ptr<ScorePlayerTeam>> teamList;

    std::unique_ptr<Collection> getTeams = std::make_unique<Collection>(Jvm::env->CallObjectMethod(this->instance, getTeamsMethodID));

    const jobjectArray array = getTeams->ToArray();

    for (jint i = 0; i < getTeams->Size(); ++i)
    {
        teamList.push_back(std::make_unique<ScorePlayerTeam>(Jvm::env->NewGlobalRef(Jvm::env->GetObjectArrayElement(array, i))));
    }

    Jvm::env->DeleteGlobalRef(array);

    return teamList;
}

void Scoreboard::RemoveTeam(const std::unique_ptr<ScorePlayerTeam>& team) const 
{
    Jvm::env->CallVoidMethod(this->instance, removeTeamMethodID, team->GetInstance());
}

void Scoreboard::RemovePlayerFromTeam(const std::string& playerName, const std::unique_ptr<ScorePlayerTeam>& team) const
{
    Jvm::env->CallVoidMethod(this->instance, removePlayerFromTeamMethodID, JavaUtil::StringToJString(playerName), team->GetInstance());
}