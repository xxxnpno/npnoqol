#pragma once

#include "../../src/wrapper/sdk/net/minecraft/client/Minecraft/Minecraft.h"

#include <print>
#include <memory>

#include <npno/Loader.hpp>

class Module
{
public:
	explicit Module(const bool enable = true);

	virtual ~Module();

	virtual void Update() = 0;

	virtual bool SanityCheck() const;

	virtual bool IsEnable() const final;

protected:
	std::unique_ptr<Minecraft> mc;

private:
	bool enable;
};