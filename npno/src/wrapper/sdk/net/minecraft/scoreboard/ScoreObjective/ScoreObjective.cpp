#include "ScoreObjective.h"

ScoreObjective::ScoreObjective(const jobject instance)
	: JavaClass("net/minecraft/scoreboard/ScoreObjective", instance)
{
	this->Init();
}

ScoreObjective::~ScoreObjective() = default;

void ScoreObjective::Init()
{
	std::call_once(oflag, [this]
		{
			getNameMethodID = Jvm::env->GetMethodID(this->javaClass, "getName", "()Ljava/lang/String;");
			getDisplayNameMethodID = Jvm::env->GetMethodID(this->javaClass, "getDisplayName", "()Ljava/lang/String;");
			setDisplayNameMethodID = Jvm::env->GetMethodID(this->javaClass, "setDisplayName", "(Ljava/lang/String;)V");
		});
}

std::string ScoreObjective::GetName() const
{
	return JavaUtil::JStringToString(static_cast<jstring>(Jvm::env->CallObjectMethod(this->instance, getNameMethodID)));
}

std::string ScoreObjective::GetDisplayName() const
{
	return JavaUtil::JStringToString(static_cast<jstring>(Jvm::env->CallObjectMethod(this->instance, getDisplayNameMethodID)));
}

void ScoreObjective::SetDisplayName(const std::string& name) const
{
	Jvm::env->CallVoidMethod(this->instance, setDisplayNameMethodID, JavaUtil::StringToJString(name));
}