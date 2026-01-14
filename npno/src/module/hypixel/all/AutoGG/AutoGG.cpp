#include "AutoGG.h"

#include "../../util/api/HypixelAPI/HypixelAPI.h"
#include "../../util/HypixelGamemode/HypixelGamemode.h"

#include <random>
#include <cctype>

hypixel::AutoGG::AutoGG()
    : HypixelStatsModule{ true, HypixelGamemode::Gamemode::ALL }
{
    Jvm::PlaceHook(
        "Lnet/minecraft/entity/Entity;",
        "addChatMessage",
        "(Lnet/minecraft/util/IChatComponent;)V",
        [this] (jthread thread)
        {
            if (this->IsEnable())
            {
                this->AddChatMessagehook(thread);
            }
        }
    );
}

hypixel::AutoGG::~AutoGG() = default;

auto hypixel::AutoGG::Update() -> void
{

}

auto hypixel::AutoGG::AddChatMessagehook(jthread thread) const -> void
{
    jobject instance{ nullptr };
    Jvm::jvmti->GetLocalObject(thread, 1, &instance);

    const std::unique_ptr<IChatComponent> chatComponent = std::make_unique<IChatComponent>(instance);
    const std::string text = chatComponent->GetFormattedText();

    bool sendMessage = false;
    for (const std::strin& line : HypixelAPI::GetAutoGGLines())
    {
        if (text.find(line) != std::string::npos)
        {
            sendMessage = true;
            break;
        }
    }

    if (sendMessage)
    {
        mc->GetThePlayer()->SendChatMessage(this->RandomCase("good game"))
    }
}

auto hypixel::AutoGG::RandomCase(const std::string& message) const -> std::string
{
    std::string result = message;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 1);
    
    for (char& c : result) 
    {
        if (std::isalpha(c) && dist(gen) == 1) 
        {
            c = std::toupper(c);
        } else 
        {
            c = std::tolower(c);
        }
    }
    
    return result;
}