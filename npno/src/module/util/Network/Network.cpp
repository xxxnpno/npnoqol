#include "Network.h"

#include "../Config/Config.h"

#include <print>
#include <thread>

#include <curl/curl.h>

auto Network::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) -> size_t
{
    size_t totalSize = size * nmemb;
    std::string* str = static_cast<std::string*>(userp);
    str->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

auto Network::Get(const std::string& endpoint) -> std::string
{
    const std::string url = std::format("{}{}{}", Network::url, Config::GetHypixelAPIKey(), endpoint);

    CURL* curl = curl_easy_init();

    std::string response;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        curl_easy_cleanup(curl);
        throw std::runtime_error(curl_easy_strerror(res));
    }

    curl_easy_cleanup(curl);

    return response;
}

auto Network::GetBatch(const std::vector<std::string>& endpoints) -> std::vector<std::string>
{
    std::vector<std::future<std::string>> futures;
    futures.reserve(endpoints.size());

    for (const auto& endpoint : endpoints)
    {
        futures.push_back(std::async(std::launch::async, [endpoint]()
            {
                try
                {
                    return Get(endpoint);
                }
                catch (...)
                {
                    return std::string{};
                }
            }));
    }

    std::vector<std::string> results;
    results.reserve(futures.size());

    for (auto& future : futures)
    {
        results.push_back(future.get());
    }

    return results;
}