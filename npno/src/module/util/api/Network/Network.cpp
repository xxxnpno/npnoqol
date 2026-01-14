#include "Network.h"

std::string Network::Get(const std::string& url)
{
    /*
    CURL* curl = curl_easy_init();
    std::string response;

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    return response;
    */

    return R"({"success":true,"player":null})";
}

size_t Network::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    std::string* s = static_cast<std::string*>(userp);
    s->append(static_cast<char*>(contents), size * nmemb);
    return size * nmemb;
}