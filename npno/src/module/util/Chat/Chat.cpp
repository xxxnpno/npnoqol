#include "Chat.h"

#include <fstream>

Chat::Chat()
{
    const char* userProfile = std::getenv("USERPROFILE");

    logFilePath = std::filesystem::path(userProfile) /
        ".lunarclient" / "profiles" / "lunar" / "1.8" / "logs" / "latest.log";

    std::ofstream ofs(logFilePath, std::ios::trunc);
    ofs.close();

    allLines.clear();
    newLines.clear();
}

Chat::~Chat() = default;

auto Chat::Update() -> void
{
    std::ifstream ifs(logFilePath);
    if (!ifs.is_open()) return;

    std::vector<std::string> currentLines;
    std::string line;
    while (std::getline(ifs, line))
    {
        currentLines.push_back(line);
    }

    newLines.clear();
    auto it = std::search(currentLines.begin(), currentLines.end(), allLines.begin(), allLines.end());

    if (it == currentLines.end())
    {
        newLines = currentLines;
    }
    else
    {
        newLines.assign(it + allLines.size(), currentLines.end());
    }

    allLines = std::move(currentLines);
}

auto Chat::GetNewLines() -> std::vector<std::string>
{
    return newLines;
}