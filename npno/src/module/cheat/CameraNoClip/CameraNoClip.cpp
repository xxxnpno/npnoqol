#include "CameraNoClip.h"

cheat::CameraNoClip::CameraNoClip()
    : Module(true)
{
    Jvm::PlaceHook(
        "Lnet/minecraft/client/multiplayer/WorldClient;",
        "rayTraceBlocks",
        "(Lnet/minecraft/util/Vec3;Lnet/minecraft/util/Vec3;)Lnet/minecraft/util/MovingObjectPosition;",
        [this] (jthread thread)
        {
            if (this->IsEnable())
            {
                this->RayTraceBlocksHook(thread);
            }
        }
    );
}

cheat::CameraNoClip::~CameraNoClip() = default;

auto cheat::CameraNoClip::Update() -> void
{

}

auto cheat::CameraNoClip::RayTraceBlocksHook(jthread thread) const -> void
{
    jvmtiFrameInfo frames[2];
    jint count{ 0 };
    Jvm::jvmti->GetStackTrace(thread, 0, 2, frames, &count);

    char* name{ nullptr };
    char* sig{ nullptr };
    char* cls{ nullptr };

    jclass declaring;
    Jvm::jvmti->GetMethodDeclaringClass(frames[1].method, &declaring);
    Jvm::jvmti->GetClassSignature(declaring, &cls, nullptr);
    Jvm::jvmti->GetMethodName(frames[1].method, &name, &sig, nullptr);

    const bool isOrientCamera =
        cls and 
        name and
        std::strcmp(cls, "Lnet/minecraft/client/renderer/EntityRenderer;") == 0 and
        std::strcmp(name, "orientCamera") == 0;

    if (name) Jvm::jvmti->Deallocate((unsigned char*)name);
    if (sig)  Jvm::jvmti->Deallocate((unsigned char*)sig);
    if (cls)  Jvm::jvmti->Deallocate((unsigned char*)cls);
    
    if (isOrientCamera)
    {
        Jvm::jvmti->ForceEarlyReturnObject(thread, nullptr);
    }
}