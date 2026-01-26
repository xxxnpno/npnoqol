#include "ChatManager.h"

hypixel::ChatManager::ChatManager()
    : HypixelStatsModule{ true, HypixelGamemode::Gamemode::ALL }
{
    this->AddBlacklistedLines({
        "Sending you to",
        "Slow down! You can only use /tip",
        "You are sending commands too fast! Please slow dowm."
    });
}

hypixel::ChatManager::~ChatManager() = default;

auto hypixel::ChatManager::Update() -> void 
{
    const std::vector<std::unique_ptr<ChatLine>>& chatLines = mc->GetIngameGUI()->GetPersistantChatGUI()->GetChatLines();

    bool refresh = false;
    if (!chatLines[0]->GetLineString()->GetFormattedText().contains(":")) 
    {
        for (const std::string& line : this->blacklistedLines) 
        {
            if (chatLines[0]->GetLineString()->GetFormattedText().find(line) != std::string::npos) 
            {
                mc->GetIngameGUI()->GetPersistantChatGUI()->DeleteChatLine(chatLines[0]->GetChatLineID());
                refresh = true; 
                break; 
            }
        }
    }

    if (refresh) 
    {
        mc->GetIngameGUI()->GetPersistantChatGUI()->RefreshChat();
    }
}

auto hypixel::ChatManager::AddBlacklistedLine(const std::string& line) -> void
{
    this->blacklistedLines.push_back(line);
}

auto hypixel::ChatManager::AddBlacklistedLines(const std::vector<std::string>& lines) -> void
{
    this->blacklistedLines.insert(this->blacklistedLines.end(), std::make_move_iterator(lines.begin()), std::make_move_iterator(lines.end()));
}
