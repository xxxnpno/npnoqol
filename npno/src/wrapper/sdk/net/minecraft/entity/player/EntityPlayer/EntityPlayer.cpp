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
	std::call_once(oflag, [this]
		{
			isSpectatorMethodID = Jvm::env->GetMethodID(this->javaClass, "isSpectator", "()Z");
			isUsingItemMethodID = Jvm::env->GetMethodID(this->javaClass, "isBlocking", "()Z");
			canAttackPlayerMethodID = Jvm::env->GetMethodID(this->javaClass, "canAttackPlayer", "(Lnet/minecraft/entity/player/EntityPlayer;)Z");
			getCustomNameTagMethodID = Jvm::env->GetMethodID(this->javaClass, "getCustomNameTag", "()Ljava/lang/String;");
			getGameProfileMethodID = Jvm::env->GetMethodID(this->javaClass, "getGameProfile", "()Lcom/mojang/authlib/GameProfile;");
		});
}

bool EntityPlayer::IsSpectator() const
{
	return Jvm::env->CallBooleanMethod(this->instance, isSpectatorMethodID);
}

bool EntityPlayer::IsUsingItem() const
{
	return Jvm::env->CallBooleanMethod(this->instance, isUsingItemMethodID);
}

bool EntityPlayer::CanAttackPlayer(const std::unique_ptr<EntityPlayer>& target) const
{
	return Jvm::env->CallBooleanMethod(this->instance, canAttackPlayerMethodID, target->GetInstance());
}

std::string EntityPlayer::GetCustomNameTag() const
{
	return JavaUtil::JStringToString(static_cast<jstring>(Jvm::env->CallObjectMethod(this->instance, getCustomNameTagMethodID)));
}

std::unique_ptr<GameProfile> EntityPlayer::GetGameProfile() const
{
	return std::make_unique<GameProfile>(Jvm::env->NewGlobalRef(Jvm::env->CallObjectMethod(this->instance, getGameProfileMethodID)));
}