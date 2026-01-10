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

		});
}