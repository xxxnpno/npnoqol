#include "CurrentGamemode.h"

#include "../../util/HypixelGamemode/HypixelGamemode.h"

hypixel::CurrentGamemode::CurrentGamemode()
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

hypixel::CurrentGamemode::~CurrentGamemode() = default;

auto hypixel::CurrentGamemode::Update() -> void
{

}

auto hypixel::CurrentGamemode::AddChatMessagehook(jthread thread) const -> void
{
    jobject instance{ nullptr };
    Jvm::jvmti->GetLocalObject(thread, 1, &instance);

    const std::unique_ptr<IChatComponent> chatComponent = std::make_unique<IChatComponent>(instance);
    const std::string text = chatComponent->GetFormattedText();

    if (nlohmann::json::accept(text))
    {
        const auto json = nlohmann::json::parse(text);

        if (json.contains("mode"))
        {
            HypixelStatsModule::SetCurrentGamemode(
                HypixelGamemode::stringToGamemode.at(json["mode"].get<std::string>())
            );
        }
    }
}