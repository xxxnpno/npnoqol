#include "Base.h"

#include "../wrapper/Jvm/Jvm.h"

#include <chrono>
#include <thread>

Base::Base()
    : running{ Jvm::Init() }
{
    if (running)
    {
        this->moduleManager = std::make_unique<ModuleManager>();
    }
}

Base::~Base()
{
    Jvm::ShutDown();
}

void Base::Run()
{
    while (this->running)
    {
        this->moduleManager->Update();

        if (GetAsyncKeyState(VK_DELETE) bitand 0x8000)
        {
            this->running = false;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}