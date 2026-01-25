#include "Command.h"

#include <sstream>
#include <print>

Command::Command(const std::string name, const std::string usage)
	: name{ name }
	, usage{ 
        std::format("{}Usage: {}",
        MinecraftCode::codeToString.at(MinecraftCode::Code::DARK_AQUA),
        usage)
    }
{
	static std::once_flag oflag;
	std::call_once(oflag, [this]
		{
			mc = std::make_unique<Minecraft>();
		});
}

Command::~Command() = default;

auto Command::Proc() -> void
{
    const std::vector<std::unique_ptr<ChatLine>>& chatLines = mc->GetIngameGUI()->GetPersistantChatGUI()->GetChatLines();

    const std::string text = chatLines[0]->GetLineString()->GetFormattedText();

	bool refresh = false;
    if (text.find(std::format("{}", this->name)) != std::string::npos and !text.contains(":"))
    {
        mc->GetIngameGUI()->GetPersistantChatGUI()->DeleteChatLine(chatLines[0]->GetChatLineID());

        this->OnCommand(this->ExtractBetweenQuotes(text));

        refresh = true;
    }

    if (refresh)
    {
        mc->GetIngameGUI()->GetPersistantChatGUI()->RefreshChat();
    }
}

std::string Command::ExtractBetweenQuotes(const std::string& text)
{
    const Size first = text.find('\'');
    if (first == std::string::npos)
    {
        return "";
    }

    const Size second = text.find('\'', first + 1);
    if (second == std::string::npos)
    {
        return "";
    }

    return text.substr(first + 1, second - first - 1);
}

std::vector<std::string> Command::GetArguments(const std::string& string)
{
    std::vector<std::string> args;
    std::istringstream stream(string);
    std::string word;

    while (stream >> word) 
    {
        args.push_back(word);
    }

    return args;
}