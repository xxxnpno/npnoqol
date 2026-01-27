#pragma once

#include "../Flag/Flag.h"

class FlagManager final
{
public:
    FlagManager();

    ~FlagManager();

    auto Update() const -> void;
private:
    template<typename F>
	auto RegisterFlag() -> void;

    std::vector<std::unique_ptr<Flag>> flags;
};