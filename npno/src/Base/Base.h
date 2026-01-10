#pragma once

#include "../module/ModuleManager/ModuleManager.h"

#include <memory>

#include <npno/Loader.hpp>

class Base final
{
public:
    Base();

    ~Base();

    void Run();

private:
    std::unique_ptr<ModuleManager> moduleManager;

    bool running;
};

REGISTER_CLASS(Base);