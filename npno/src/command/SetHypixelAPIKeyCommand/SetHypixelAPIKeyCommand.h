#pragma once

#include "../Command/Command.h"

class SetHypixelAPIKeyCommand final : public Command
{
public:
	SetHypixelAPIKeyCommand();

	~SetHypixelAPIKeyCommand() override;

	auto OnCommand(const std::string& input) -> void override;
};