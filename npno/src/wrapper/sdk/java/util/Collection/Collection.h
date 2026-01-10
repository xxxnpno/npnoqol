#pragma once

#include "../../src/wrapper/JavaClass/JavaClass.h"

class Collection : public JavaClass
{
public:
	explicit Collection(const jobject instance);
	Collection(const char* name, const jobject instance);

	virtual ~Collection() override;

	virtual void Init() override;

	[[nodiscard]] virtual jint Size() const final;

	[[nodiscard]] virtual jobjectArray ToArray() const final;

private:
	inline static jmethodID sizeMethodID{ nullptr };
	inline static jmethodID toArrayMethodID{ nullptr };
};