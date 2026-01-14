#pragma once

#include "../../Module/Module.h"

namespace cheat
{
	class CameraNoClip final : public Module
	{
	public:
		CameraNoClip();

		~CameraNoClip() override;

		auto Update() -> void override;

    private:
        auto RayTraceBlocksHook(jthread thread) const -> void;
	};
}