#include "Entity.h"

Entity::Entity(const jobject instance)
	: JavaClass("net/minecraft/entity/Entity", instance)
{
	this->Init();
}

Entity::Entity(const char* name, const jobject instance)
	: JavaClass(name, instance)
{
	this->Init();
}

Entity::~Entity() = default;

void Entity::Init()
{
	std::call_once(oflag, [this]
		{
			isSprintingMethodID = Jvm::env->GetMethodID(this->javaClass, "isSprinting", "()Z");
			isSneakingMethodID = Jvm::env->GetMethodID(this->javaClass, "isSneaking", "()Z");
			isInvisibleMethodID = Jvm::env->GetMethodID(this->javaClass, "isInvisible", "()Z");
			isEatingMethodID = Jvm::env->GetMethodID(this->javaClass, "isEating", "()Z");
			isRidingMethodID = Jvm::env->GetMethodID(this->javaClass, "isRiding", "()Z");
			getUniqueIDMethodID = Jvm::env->GetMethodID(this->javaClass, "getUniqueID", "()Ljava/util/UUID;");
			getNameMethodID = Jvm::env->GetMethodID(this->javaClass, "getName", "()Ljava/lang/String;");
			addChatMessageMethodID = Jvm::env->GetMethodID(this->javaClass, "addChatMessage", "(Lnet/minecraft/util/IChatComponent;)V");

			onGroundFieldID = Jvm::env->GetFieldID(this->javaClass, "onGround", "Z");
			rotationYawFieldID = Jvm::env->GetFieldID(this->javaClass, "rotationYaw", "F");
			rotationPitchFieldID = Jvm::env->GetFieldID(this->javaClass, "rotationPitch", "F");
			prevRotationYawFieldID = Jvm::env->GetFieldID(this->javaClass, "prevRotationYaw", "F");
			prevRotationPitchFieldID = Jvm::env->GetFieldID(this->javaClass, "prevRotationPitch", "F");
			posXFieldID = Jvm::env->GetFieldID(this->javaClass, "posX", "D");
			posYFieldID = Jvm::env->GetFieldID(this->javaClass, "posY", "D");
			posZFieldID = Jvm::env->GetFieldID(this->javaClass, "posZ", "D");
			lastTickPosXFieldID = Jvm::env->GetFieldID(this->javaClass, "lastTickPosX", "D");
			lastTickPosYFieldID = Jvm::env->GetFieldID(this->javaClass, "lastTickPosY", "D");
			lastTickPosZFieldID = Jvm::env->GetFieldID(this->javaClass, "lastTickPosZ", "D");
			motionXFieldID = Jvm::env->GetFieldID(this->javaClass, "motionX", "D");
			motionYFieldID = Jvm::env->GetFieldID(this->javaClass, "motionY", "D");
			motionZFieldID = Jvm::env->GetFieldID(this->javaClass, "motionZ", "D");
		});
}

bool Entity::IsSprinting() const
{
	return Jvm::env->CallBooleanMethod(this->instance, isSprintingMethodID);
}

bool Entity::IsSneaking() const
{
	return Jvm::env->CallBooleanMethod(this->instance, isSneakingMethodID);
}

bool Entity::IsInvisible() const
{
	return Jvm::env->CallBooleanMethod(this->instance, isInvisibleMethodID);
}

bool Entity::IsEating() const
{
	return Jvm::env->CallBooleanMethod(this->instance, isEatingMethodID);
}

bool Entity::IsRiding() const
{
	return Jvm::env->CallBooleanMethod(this->instance, isRidingMethodID);
}

bool Entity::IsOnGround() const
{
	return Jvm::env->GetBooleanField(this->instance, onGroundFieldID);
}

float Entity::GetRotationYaw() const
{
	return Jvm::env->GetFloatField(this->instance, rotationYawFieldID);
}

float Entity::GetRotationPitch() const
{
	return Jvm::env->GetFloatField(this->instance, rotationPitchFieldID);
}

float Entity::GetLastRotationYaw() const
{
	return Jvm::env->GetFloatField(this->instance, prevRotationYawFieldID);
}

float Entity::GetLastRotationPitch() const
{
	return Jvm::env->GetFloatField(this->instance, prevRotationPitchFieldID);
}

std::unique_ptr<UUID_J> Entity::GetUniqueID() const
{
	return std::make_unique<UUID_J>(Jvm::env->NewGlobalRef(Jvm::env->CallObjectMethod(this->instance, getUniqueIDMethodID)));
}

std::string Entity::GetName() const
{
	return JavaUtil::JStringToString(static_cast<jstring>(Jvm::env->CallObjectMethod(this->instance, getNameMethodID)));
}

void Entity::AddChatMessage(const std::unique_ptr<IChatComponent>& chatComponent) const
{
	Jvm::env->CallVoidMethod(this->instance, addChatMessageMethodID, chatComponent->GetInstance());
}

double Entity::GetPosX() const
{
	return Jvm::env->GetDoubleField(this->instance, posXFieldID);
}

double Entity::GetPosY() const
{
	return Jvm::env->GetDoubleField(this->instance, posYFieldID);
}

double Entity::GetPosZ() const
{
	return Jvm::env->GetDoubleField(this->instance, posZFieldID);
}

double Entity::GetLastTickPosX() const
{
	return Jvm::env->GetDoubleField(this->instance, lastTickPosXFieldID);
}

double Entity::GetLastTickPosY() const
{
	return Jvm::env->GetDoubleField(this->instance, lastTickPosYFieldID);
}

double Entity::GetLastTickPosZ() const
{
	return Jvm::env->GetDoubleField(this->instance, lastTickPosZFieldID);
}

double Entity::GetMotionX() const
{
	return Jvm::env->GetDoubleField(this->instance, motionXFieldID);
}

double Entity::GetMotionY() const
{
	return Jvm::env->GetDoubleField(this->instance, motionYFieldID);
}

double Entity::GetMotionZ() const
{
	return Jvm::env->GetDoubleField(this->instance, motionZFieldID);
}