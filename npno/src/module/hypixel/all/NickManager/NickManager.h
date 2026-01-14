#pragma once

#include "../../HypixelStatsModule/HypixelStatsModule.h"

class NickManager final : public HypixelStatsModule
{
public:
    NickManager();

    ~NickManager() override();

    auto Update() -> void override;

private:
    auto Warn(const std::string& name) -> void;
}