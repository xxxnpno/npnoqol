#pragma once

#include "../../src/wrapper/sdk/java/util/Collection/Collection.h"

class List final : public Collection
{
public:
	explicit List(const jobject instance);

	virtual ~List() override;

	virtual void Init() override;

	[[nodiscard]] jobject Get(const I32 index) const;

private:
	inline static std::once_flag oflag{};

	inline static jmethodID getMethodID{ nullptr };
};