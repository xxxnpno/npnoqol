#include "Network.h"

#include "../Config/Config.h"
#include "../api/HypixelAPI/HypixelAPI.h"

#include <print>
#include <thread>
#include <chrono>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

auto Network::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) -> size_t
{
    size_t totalSize = size * nmemb;
    std::string* str = static_cast<std::string*>(userp);
    str->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

auto Network::Get(const std::string& endpoint, int maxRetries) -> std::string
{
    const std::string url = std::format("{}{}", Network::url, endpoint);

    for (I32 attempt = 0; attempt < maxRetries; ++attempt)
    {
        CURL* curl = curl_easy_init();
        std::string response;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res == CURLE_OK)
        {
            return response;
        }

        if (attempt < maxRetries - 1)
        {
            I32 delayMs = BASE_DELAY_MS * (1 << attempt);
            std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        }
    }
}

auto Network::GetBatchPlayerStats(const std::vector<std::string>& players) -> std::vector<nlohmann::json>
{
    std::vector<std::future<nlohmann::json>> futures;
    futures.reserve(players.size());

    for (const std::string& player : players)
    {
        futures.push_back(std::async(std::launch::async, [player]()
            {
                try
                {
                    return HypixelAPI::GetPlayerStats(player);
                }
                catch (...)
                {
                    return nlohmann::json{};
                }
            }));
    }

    std::vector<nlohmann::json> results;
    results.reserve(futures.size());

    for (auto& future : futures)
    {
        results.push_back(future.get());
    }

    return results;
}