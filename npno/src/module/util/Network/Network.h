#pragma once

#include <string>
#include <vector>
#include <future>

#include <nlohmann/json.hpp>

class Network final
{
public:
    [[nodiscard]] static auto Get(const std::string& endpoint) -> std::string;

    [[nodiscard]] static auto GetBatchPlayerStats(const std::vector<std::string>& players) -> std::vector<nlohmann::json>;

private:
    static auto WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) -> size_t;

    inline static std::string url = "https://api.hypixel.net/";
};