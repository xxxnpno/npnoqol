#include "EntityPlayer.h"

EntityPlayer::EntityPlayer(const jobject instance)
	: EntityLivingBase("net/minecraft/entity/player/EntityPlayer", instance)
{
	this->Init();
}

EntityPlayer::EntityPlayer(const char* name, const jobject instance)
	: EntityLivingBase(name, instance)
{
	this->Init();
}

EntityPlayer::~EntityPlayer() = default;

void EntityPlayer::Init()
{
	std::call_once(this->entityPlayerSPOflag, [this]
		{
			isSpectatorMethodID = Jvm::env->GetMethodID(this->javaClass, "isSpectator", "()Z");
			getCustomNameTagMethodID = Jvm::env->GetMethodID(this->javaClass, "getCustomNameTag", "()Ljava/lang/String;");
			canAttackPlayerMethodID = Jvm::env->GetMethodID(this->javaClass, "canAttackPlayer", "(Lnet/minecraft/entity/player/EntityPlayer;)Z");
		});
}

bool EntityPlayer::IsSpectator() const
{
	return Jvm::env->CallBooleanMethod(this->instance, isSpectatorMethodID);
}

std::string EntityPlayer::GetCustomNameTag() const
{
	return JavaUtil::JStringToString(static_cast<jstring>(Jvm::env->CallObjectMethod(this->instance, getCustomNameTagMethodID)));
}

bool EntityPlayer::CanAttackPlayer(const std::unique_ptr<EntityPlayer>& target) const
{
	return Jvm::env->CallBooleanMethod(this->instance, canAttackPlayerMethodID, target->GetInstance());
}