#include "Network.h"


#include <string>
#include <sstream>

#include <winsock2.h>
#include <ws2tcpip.h>

std::string Network::Get(const std::string& endpoint) 
{
    try 
    {
        const std::string host = url;
        const std::string path = endpoint.empty() ? "/" : endpoint;

        WSADATA wsa;
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return "";

        struct addrinfo hints = {}, *res;
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        if (getaddrinfo(host.c_str(), "80", &hints, &res) != 0) 
        {
            WSACleanup();
            return "";
        }

        SOCKET sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sock == INVALID_SOCKET) 
        {
            freeaddrinfo(res);
            WSACleanup();
            return "";
        }

        if (connect(sock, res->ai_addr, (int)res->ai_addrlen) != 0) 
        {
            closesocket(sock);
            freeaddrinfo(res);
            WSACleanup();
            return "";
        }

        std::ostringstream request;
        request << "GET " << path << " HTTP/1.1\r\n";
        request << "Host: " << host << "\r\n";
        request << "Connection: close\r\n\r\n";
        std::string reqStr = request.str();
        send(sock, reqStr.c_str(), static_cast<int>(reqStr.size()), 0);

        std::string response;
        char buffer[4096];
        int bytesReceived;
        while ((bytesReceived = recv(sock, buffer, sizeof(buffer)-1, 0)) > 0) 
        {
            buffer[bytesReceived] = '\0';
            response += buffer;
        }

        closesocket(sock);
        freeaddrinfo(res);
        WSACleanup();

        auto pos = response.find("\r\n\r\n");
        if (pos != std::string::npos) return response.substr(pos + 4);
        return response;
    } catch (...) 
    {
        return "";
    }
}
