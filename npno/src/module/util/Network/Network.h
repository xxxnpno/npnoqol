#pragma once

#include <string>

class Network final 
{
public:
    [[nodiscard]] static std::string Get(const std::string& endpoint);

private:
    inline static std::string url = "www.hypixel.net";
};
