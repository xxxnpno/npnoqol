#pragma once

#include <string>

class Config final
{
public:
	static auto GetHypixelAPIKey() -> std::string;

	static auto SetHypixelAPIKey(const std::string& apiKey) -> void;
};