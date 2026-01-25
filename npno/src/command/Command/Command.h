#pragma once

#include "../../src/wrapper/sdk/net/minecraft/client/Minecraft/Minecraft.h"

#include "../../util/api/HypixelAPI/HypixelAPI.h"
#include "../../util/MinecraftCode/MinecraftCode.h"

#include <print>
#include <memory>

#include <npno/Loader.hpp>

class Command
{
public:
	Command(const std::string name = "", const std::string usage = "");

	virtual ~Command();

	virtual auto OnCommand(const std::string& input) -> void = 0;

	virtual auto Proc() -> void;

protected:
	std::string ExtractBetweenQuotes(const std::string& text);
	std::vector<std::string> GetArguments(const std::string& string);

	inline static std::unique_ptr<Minecraft> mc{ nullptr };

	std::string name;
	std::string usage;
};