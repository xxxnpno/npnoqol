#pragma once

#include <string>
#include <vector>
#include <future>

class Network final
{
public:
    [[nodiscard]] static auto Get(const std::string& endpoint) -> std::string;

    [[nodiscard]] static auto GetBatch(const std::vector<std::string>& endpoints) -> std::vector<std::string>;

private:
    static auto WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) -> size_t;

    inline static std::string url = "https://api.hypixel.net/player?key=";
};