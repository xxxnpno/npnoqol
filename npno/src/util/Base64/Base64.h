#pragma once

#include <string>

class Base64
{
public:
    static std::string Decode(const std::string& input);

private:
    static bool IsBase64(unsigned char c);

    inline static const std::string base64_chars{ "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/" };
};