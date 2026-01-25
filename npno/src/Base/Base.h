#pragma once

#include "../module/ModuleManager/ModuleManager.h"
#include "../command/CommandManager/CommandManager.h"

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
    std::unique_ptr<CommandManager> commandManager;

    bool running;
};

REGISTER_CLASS(Base);