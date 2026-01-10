#include "List.h"

List::List(const jobject instance)
	: Collection("java/util/List", instance)
{
	this->Init();
}

List::~List() = default;

void List::Init()
{
	std::call_once(this->listOFlag, [this]
		{
			getMethodID = Jvm::env->GetMethodID(this->javaClass, "get", "(I)Ljava/lang/Object;");
		});
}

jobject List::Get(const I32 index) const
{
	return Jvm::env->CallObjectMethod(this->instance, getMethodID, index);
}