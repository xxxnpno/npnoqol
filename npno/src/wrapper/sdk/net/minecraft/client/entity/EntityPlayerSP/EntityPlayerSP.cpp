#include "EntityPlayerSP.h"

#include "../../src/wrapper/JavaUtil/JavaUtil.h"

EntityPlayerSP::EntityPlayerSP(const jobject instance)
    : EntityPlayer("net/minecraft/client/entity/EntityPlayerSP", instance)
{
    this->Init();
}

EntityPlayerSP::~EntityPlayerSP() = default;

void EntityPlayerSP::Init() 
{
    std::call_once(oflag, [this] 
        {
        sendQueueFieldID = Jvm::env->GetFieldID(this->javaClass, "sendQueue", "Lnet/minecraft/client/network/NetHandlerPlayClient;");

        sendChatMessageMethodID = Jvm::env->GetMethodID(this->javaClass, "sendChatMessage", "(Ljava/lang/String;)V");
        });
}

std::unique_ptr<NetHandlerPlayClient> EntityPlayerSP::GetSendQueue() const 
{
    return std::make_unique<NetHandlerPlayClient>(Jvm::env->NewGlobalRef(Jvm::env->GetObjectField(this->instance, sendQueueFieldID)));
}

void EntityPlayerSP::SendChatMessage(const std::string& message) const
{
    Jvm::env->CallVoidMethod(this->instance, sendChatMessageMethodID, JavaUtil::StringToJString(JavaUtil::FixString(message)));
}