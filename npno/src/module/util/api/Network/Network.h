#pragma once

#include <string>

class Network final
{
public:
	static std::string Get(const std::string& url);

private:
	static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
};