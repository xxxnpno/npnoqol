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

	virtual void Update() = 0;

	virtual auto SanityCheck() const -> bool;

	virtual bool IsEnable() const final;
	virtual void SetEnable(const bool state) final;

protected:
	inline static std::unique_ptr<Minecraft> mc{ nullptr };

private:
	bool enable;
};