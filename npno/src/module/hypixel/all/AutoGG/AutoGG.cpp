#include "AutoGG.h"

#include "../../../util/api/HypixelAPI/HypixelAPI.h"
#include "../../../util/HypixelGamemode/HypixelGamemode.h"

#include <random>
#include <cctype>

hypixel::AutoGG::AutoGG()
    : HypixelStatsModule{ true, HypixelGamemode::Gamemode::ALL }
{
    
}

hypixel::AutoGG::~AutoGG() = default;

auto hypixel::AutoGG::Update() -> void
{
	const std::vector<std::string> newLines = Chat::GetNewLines();

    for(const std::string& line : newLines)
    {
        for (const std::string& autoGGLine : HypixelAPI::GetAutoGGLines())
        {
            if (line.find(autoGGLine) != std::string::npos and this->SentByServer(line))
            {
                const std::string randomizedMessage = std::format("{}{}", "/ac", this->RandomCase("good game"));
                mc->GetThePlayer()->SendChatMessage(randomizedMessage);
                return;
            }
        }
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

auto hypixel::AutoGG::SentByServer(const std::string& line) const -> bool
{
    I32 count = 0;
    for (const char c : line)
    {
        if (c == ':') 
        {
            count++;
        }
    }
    return count;
}