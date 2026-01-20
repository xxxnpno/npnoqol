#pragma once

#include "../../src/wrapper/sdk/net/minecraft/client/Minecraft/Minecraft.h"

#include "../util/Chat/Chat.h"

#include <print>
#include <memory>

#include <npno/Loader.hpp>

class Module
{
public:
	explicit Module(const bool enable = true);

	virtual ~Module();

	virtual auto Update() = 0 -> void;

	virtual auto SanityCheck() const -> bool;

	virtual auto IsEnable() const -> bool final;
	virtual auto SetEnable(const bool state) -> void final;

protected:
	inline static std::unique_ptr<Minecraft> mc{ nullptr };

private:
	bool enable;
};