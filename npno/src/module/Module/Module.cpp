#include "Module.h"

#include <print>

Module::Module(const bool enable)
	: enable{ enable }
	, mc{ std::make_unique<Minecraft>() }
{
	std::println("[INFO] Minecraft instance acquired: {}", static_cast<void*>(this->mc->GetInstance()));
}

Module::~Module() = default;

auto Module::SanityCheck() const -> bool
{
	return this->mc->GetTheWorld()->GetInstance() and this->mc->GetThePlayer()->GetInstance();
}

bool Module::IsEnable() const
{
	return this->enable;
}