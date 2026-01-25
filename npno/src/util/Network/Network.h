#pragma once

#include <string>
#include <vector>
#include <future>

#include <npno/Loader.hpp>
#include <nlohmann/json.hpp>

class Network final
{
public:
    [[nodiscard]] static auto Get(const std::string& endpoint, int maxRetries = 3) -> std::string;

    [[nodiscard]] static auto GetBatchPlayerStats(const std::vector<std::string>& players) -> std::vector<nlohmann::json>;

private:
    static auto WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) -> size_t;

    inline static std::string url = "https://api.hypixel.net";

    inline static constexpr I32 MAX_RETRIES = 3;
    inline static constexpr I32 BASE_DELAY_MS = 100;
};