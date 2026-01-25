#include "Score.h"

Score::Score(const jobject instance)
	: JavaClass("net/minecraft/scoreboard/Score", instance)
{
	this->Init();
}

Score::~Score() = default;

void Score::Init()
{
	std::call_once(this->oflag, [this]
		{
			getScorePointsMethodID = Jvm::env->GetMethodID(this->javaClass, "getScorePoints", "()I");
			getPlayerNameMethodID = Jvm::env->GetMethodID(this->javaClass, "getPlayerName", "()Ljava/lang/String;");
		});
}

I32 Score::GetScorePoints() const
{
	return static_cast<I32>(Jvm::env->CallIntMethod(this->instance, getScorePointsMethodID));
}

std::string Score::GetPlayerName() const
{
	return JavaUtil::JStringToString(static_cast<jstring>(Jvm::env->CallObjectMethod(this->instance, getPlayerNameMethodID)));
}