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
    std::call_once(oflag, [this]
        {
            addPlayerToTeamMethodID = Jvm::env->GetMethodID(this->javaClass, "addPlayerToTeam", "(Ljava/lang/String;Ljava/lang/String;)Z");
            getTeamMethodID = Jvm::env->GetMethodID(this->javaClass, "getTeam", "(Ljava/lang/String;)Lnet/minecraft/scoreboard/ScorePlayerTeam;");
            getPlayersTeamMethodID = Jvm::env->GetMethodID(this->javaClass, "getPlayersTeam", "(Ljava/lang/String;)Lnet/minecraft/scoreboard/ScorePlayerTeam;");
            createTeamMethodID = Jvm::env->GetMethodID(this->javaClass, "createTeam", "(Ljava/lang/String;)Lnet/minecraft/scoreboard/ScorePlayerTeam;");
            getObjectiveInDisplaySlotMethodID = Jvm::env->GetMethodID(this->javaClass, "getObjectiveInDisplaySlot", "(I)Lnet/minecraft/scoreboard/ScoreObjective;");
			getTeamsMethodID = Jvm::env->GetMethodID(this->javaClass, "getTeams", "()Ljava/util/Collection;");
            getSortedScoresMethodID = Jvm::env->GetMethodID(this->javaClass, "getSortedScores", "(Lnet/minecraft/scoreboard/ScoreObjective;)Ljava/util/Collection;");
            removeTeamMethodID = Jvm::env->GetMethodID(this->javaClass, "removeTeam", "(Lnet/minecraft/scoreboard/ScoreObjective;)Ljava/util/Collection;");
            removePlayerFromTeamMethodID = Jvm::env->GetMethodID(this->javaClass, "removePlayerFromTeam", "(Ljava/lang/String;Lnet/minecraft/scoreboard/ScorePlayerTeam;)V");
            setObjectiveInDisplaySlotMethodID = Jvm::env->GetMethodID(this->javaClass, "setObjectiveInDisplaySlot", "(ILnet/minecraft/scoreboard/ScoreObjective;)V");
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

std::unique_ptr<ScoreObjective> Scoreboard::GetObjectiveInDisplaySlot(DisplaySlot slot) const
{
    return std::make_unique<ScoreObjective>(Jvm::env->NewGlobalRef(Jvm::env->CallObjectMethod(this->instance, getObjectiveInDisplaySlotMethodID, static_cast<jint>(slot))));
}

std::vector<std::unique_ptr<ScorePlayerTeam>> Scoreboard::GetTeams() const
{
    std::vector<std::unique_ptr<ScorePlayerTeam>> teamList;

    const jobject collectionLocal = Jvm::env->CallObjectMethod(this->instance, getTeamsMethodID);

    const std::unique_ptr<Collection> getTeams = std::make_unique<Collection>(collectionLocal);
    const jobjectArray arrayLocal = static_cast<jobjectArray>(getTeams->ToArray());

    for (jint i = 0; i < getTeams->Size(); ++i)
    {
        const jobject elementLocal = Jvm::env->GetObjectArrayElement(arrayLocal, i);
        teamList.push_back(std::make_unique<ScorePlayerTeam>(elementLocal));
        Jvm::env->DeleteLocalRef(elementLocal);
    }

    Jvm::env->DeleteLocalRef(arrayLocal);
    Jvm::env->DeleteLocalRef(collectionLocal);

    return teamList;
}

std::vector<std::unique_ptr<Score>> Scoreboard::GetSortedScores(const std::unique_ptr<ScoreObjective>& objective) const
{
    std::vector<std::unique_ptr<Score>> scoreList;

    std::unique_ptr<Collection> getScores = std::make_unique<Collection>(Jvm::env->CallObjectMethod(this->instance, getSortedScoresMethodID));

    const jobjectArray array = getScores->ToArray();

    for (jint i = 0; i < getScores->Size(); ++i)
    {
        scoreList.push_back(std::make_unique<Score>(Jvm::env->NewGlobalRef(Jvm::env->GetObjectArrayElement(array, i))));
    }

    Jvm::env->DeleteGlobalRef(array);

    return scoreList;
}

void Scoreboard::RemoveTeam(const std::unique_ptr<ScorePlayerTeam>& team) const 
{
    Jvm::env->CallVoidMethod(this->instance, removeTeamMethodID, team->GetInstance());
}

void Scoreboard::RemovePlayerFromTeam(const std::string& playerName, const std::unique_ptr<ScorePlayerTeam>& team) const
{
    Jvm::env->CallVoidMethod(this->instance, removePlayerFromTeamMethodID, JavaUtil::StringToJString(playerName), team->GetInstance());
}

void Scoreboard::SetObjectiveInDisplaySlot(const DisplaySlot slot, const std::unique_ptr<ScoreObjective>& objective) const
{
    Jvm::env->CallVoidMethod(this->instance, setObjectiveInDisplaySlotMethodID, static_cast<jint>(slot), objective ? objective->GetInstance() : nullptr);
}