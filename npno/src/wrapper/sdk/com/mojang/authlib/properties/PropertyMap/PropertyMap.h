#pragma once

#include "../../src/wrapper/JavaClass/JavaClass.h"

#include "../../src/wrapper/sdk/com/mojang/authlib/properties/Property/Property.h"

class PropertyMap final : public JavaClass
{
public:
    explicit PropertyMap(const jobject instance);

    virtual ~PropertyMap() override;

    virtual void Init() override;

    [[nodiscard]] std::vector<std::unique_ptr<Property>> GetValues() const;

private:
    inline static jmethodID valuesMethodID{ nullptr };
};