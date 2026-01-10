#pragma once

#include "../../src/wrapper/JavaClass/JavaClass.h"

class GuiNewChat final : public JavaClass
{
public:
	explicit GuiNewChat(const jobject instance);

	virtual ~GuiNewChat() override;

	virtual void Init() override;

private:

};