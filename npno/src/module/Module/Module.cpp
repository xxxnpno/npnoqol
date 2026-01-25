#include "Module.h"

#include <print>

Module::Module(const bool enable)
	: enable{ enable }
{
	static std::once_flag oflag;
	std::call_once(oflag, [this]
		{
			mc = std::make_unique<Minecraft>();

			std::println("[INFO] Minecraft instance acquired: {}", static_cast<void*>(mc->GetInstance()));
		});
}

Module::~Module() = default;

auto Module::SanityCheck() -> bool
{
	return mc->GetTheWorld()->GetInstance() and mc->GetThePlayer()->GetInstance();
}

auto Module::IsEnable() const -> bool
{
	return this->enable;
}

auto Module::SetEnable(const bool state) -> void
{
	this->enable = state;
}