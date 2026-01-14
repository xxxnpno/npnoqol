#include "Module.h"

#include <print>

Module::Module(const bool enable)
	: enable{ enable }
{
	static std::once_flag oflag;
	std::call_once(oflag, [this]
		{
			mc = std::make_unique<Minecraft>();

			std::println("[INFO] Minecraft instance acquired: {}", static_cast<void*>(this->mc->GetInstance()));
		});
}

Module::~Module() = default;

auto Module::SanityCheck() const -> bool
{
	return mc->GetTheWorld()->GetInstance() and mc->GetThePlayer()->GetInstance();
}

bool Module::IsEnable() const
{
	return this->enable;
}

void Module::SetEnable(const bool state)
{
	this->enable = state;
}