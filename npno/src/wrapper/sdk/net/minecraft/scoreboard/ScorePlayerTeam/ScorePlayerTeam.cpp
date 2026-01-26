#include "ScorePlayerTeam.h"

#include "../../src/wrapper/sdk/java/util/Collection/Collection.h"

ScorePlayerTeam::ScorePlayerTeam(const jobject instance)
    : JavaClass("net/minecraft/scoreboard/ScorePlayerTeam", instance)
{
    this->Init();
}

ScorePlayerTeam::~ScorePlayerTeam() = default;

void ScorePlayerTeam::Init()
{
    std::call_once(oflag, [this]
        {
            getNamePrefixMethodID = Jvm::env->GetMethodID(this->javaClass, "getColorPrefix", "()Ljava/lang/String;");
            getNameSuffixMethodID = Jvm::env->GetMethodID(this->javaClass, "getColorSuffix", "()Ljava/lang/String;");
			getTeamNameMethodID = Jvm::env->GetMethodID(this->javaClass, "getTeamName", "()Ljava/lang/String;");
			getMembershipCollectionMethodID = Jvm::env->GetMethodID(this->javaClass, "getMembershipCollection", "()Ljava/util/Collection;");
            setNamePrefixMethodID = Jvm::env->GetMethodID(this->javaClass, "setNamePrefix", "(Ljava/lang/String;)V");
            setNameSuffixMethodID = Jvm::env->GetMethodID(this->javaClass, "setNameSuffix", "(Ljava/lang/String;)V");
        });
}

std::string ScorePlayerTeam::GetTeamName() const 
{
    return JavaUtil::JStringToString(static_cast<jstring>(Jvm::env->CallObjectMethod(this->instance, getTeamNameMethodID)));
}

std::string ScorePlayerTeam::GetNamePrefix() const 
{
    return JavaUtil::JStringToString(static_cast<jstring>(Jvm::env->CallObjectMethod(this->instance, getNamePrefixMethodID)));
}

std::string ScorePlayerTeam::GetNameSuffix() const 
{
    return JavaUtil::JStringToString(static_cast<jstring>(Jvm::env->CallObjectMethod(this->instance, getNameSuffixMethodID)));
}

std::vector<std::string> ScorePlayerTeam::GetMembershipCollection() const 
{
    std::vector<std::string> members;

    std::unique_ptr<Collection> membershipCollection = std::make_unique<Collection>(Jvm::env->CallObjectMethod(this->instance, getMembershipCollectionMethodID));
    const jobjectArray array = membershipCollection->ToArray();

    for (jint i = 0; i < membershipCollection->Size(); ++i)
    {
        members.push_back(JavaUtil::JStringToString(static_cast<jstring>(Jvm::env->NewGlobalRef(Jvm::env->GetObjectArrayElement(array, i)))));
    }

    Jvm::env->DeleteGlobalRef(array);

    return members;
}

void ScorePlayerTeam::SetNamePrefix(const std::string& prefix) const
{
    Jvm::env->CallVoidMethod(this->instance, setNamePrefixMethodID, JavaUtil::StringToJString(prefix));
}

void ScorePlayerTeam::SetNameSuffix(const std::string& suffix) const
{
    Jvm::env->CallVoidMethod(this->instance, setNameSuffixMethodID, JavaUtil::StringToJString(suffix));
}