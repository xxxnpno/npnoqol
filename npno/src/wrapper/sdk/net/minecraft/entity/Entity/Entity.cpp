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
	std::call_once(this->oflag, [this]
		{
			isInvisibleMethodID = Jvm::env->GetMethodID(this->javaClass, "isInvisible", "()Z");
			getUniqueIDMethodID = Jvm::env->GetMethodID(this->javaClass, "getUniqueID", "()Ljava/util/UUID;");
			getNameMethodID = Jvm::env->GetMethodID(this->javaClass, "getName", "()Ljava/lang/String;");
			addChatMessageMethodID = Jvm::env->GetMethodID(this->javaClass, "addChatMessage", "(Lnet/minecraft/util/IChatComponent;)V");
		});
}

bool Entity::IsInvisible() const
{
	return Jvm::env->CallBooleanMethod(this->instance, isInvisibleMethodID);
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