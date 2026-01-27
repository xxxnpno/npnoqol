#pragma once

#include "../../src/wrapper/JavaClass/JavaClass.h"

class WorldInfo : public JavaClass
{
public:
	explicit WorldInfo(const jobject instance);

	WorldInfo(const char* name, const jobject instance);

	virtual ~WorldInfo() override;

	virtual void Init() override;

    [[nodiscard]] I64 GetRandomSeed() const;
    [[nodiscars]] I64 GetWorldTime() const;

    [[nodiscard]] std::string GetWorldName() const;

private:
    inline static std::once_flag oflag{};

    inline static jfieldID randomSeedFieldID{ nullptr };
    inline static jfieldID worldTimeFieldID{ nullptr };
    inline static jfieldID worldNameFieldID{ nullptr };
};