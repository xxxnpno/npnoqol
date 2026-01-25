#include "SetHypixelAPIKeyCommand.h"

SetHypixelAPIKeyCommand::SetHypixelAPIKeyCommand()
	: Command{ 
		"sethypixelapikey",
		std::format("{}/sethypixelapikey {}<hypixel api key>", 
			MinecraftCode::codeToString.at(MinecraftCode::Code::DARK_AQUA),
			MinecraftCode::codeToString.at(MinecraftCode::Code::AQUA))
		}
{

}

SetHypixelAPIKeyCommand::~SetHypixelAPIKeyCommand() = default;

auto SetHypixelAPIKeyCommand::OnCommand(const std::string& input) -> void
{
	const std::vector<std::string> args = this->GetArguments(input);

	if (args[0] != this->name)
	{
		return;
	}

	if (args.size() != 2)
	{
		mc->GetThePlayer()->AddChatMessage(std::make_unique<ChatComponentText>(this->usage));
		return;
	}

	HypixelAPI::SetAPIKey(args[1]);
	mc->GetThePlayer()->AddChatMessage(std::make_unique<ChatComponentText>(
		std::format("{}Set hypixel api key to {}{}",
			MinecraftCode::codeToString.at(MinecraftCode::Code::DARK_AQUA), 
			args[1],
			MinecraftCode::codeToString.at(MinecraftCode::Code::AQUA))));
}