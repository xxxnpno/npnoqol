#pragma once

#include <filesystem>

class Chat final
{
public:
	Chat();

	~Chat();

	auto Update() -> void;

	static auto GetNewLines() -> std::vector<std::string>;

private:
	inline static std::vector<std::string> newLines;
	inline static std::vector<std::string> allLines;

	std::filesystem::path logFilePath;
};