#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <httplib/httplib.h>

#include <memory>
#include <thread>

namespace npno 
{
    using I8 = int8_t;
    using I16 = int16_t;
    using I32 = int32_t;
    using I64 = int64_t;

    using U8 = uint8_t;
    using U16 = uint16_t;
    using U32 = uint32_t;
    using U64 = uint64_t;

    using Size = size_t;

    template<typename Base>
    class Loader final 
    {
    public:
        static DWORD WINAPI ThreadEntry(const HMODULE module)
        {
            FILE* outputBuffer = nullptr;

            AllocConsole();
            freopen_s(&outputBuffer, "CONOUT$", "w", stdout);

            {
                const std::unique_ptr<Base> base = std::make_unique<Base>();
                base->Run();
            }

            if (outputBuffer)
            {
                fclose(outputBuffer);
            }

            FreeConsole();
            FreeLibraryAndExitThread(module, 0ul);
            return 0l;
        }
    };
}

#define REGISTER_CLASS(className)                                                               \
    BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)         \
    {                                                                                           \
        switch (ul_reason_for_call)                                                             \
        {                                                                                       \
        case DLL_PROCESS_ATTACH:                                                                \
        {                                                                                       \
            DisableThreadLibraryCalls(hModule);                                                 \
            HANDLE threadHandle = CreateThread(                                                 \
                nullptr,                                                                        \
                0ull,                                                                           \
                reinterpret_cast<LPTHREAD_START_ROUTINE>(npno::Loader<className>::ThreadEntry), \
                hModule,                                                                        \
                0ul,                                                                            \
                nullptr                                                                         \
            );                                                                                  \
            if (threadHandle)                                                                   \
            {                                                                                   \
                CloseHandle(threadHandle);                                                      \
            }                                                                                   \
            break;                                                                              \
        }                                                                                       \
        case DLL_PROCESS_DETACH:                                                                \
            break;                                                                              \
        }                                                                                       \
        return TRUE;                                                                            \
    }

using namespace npno;