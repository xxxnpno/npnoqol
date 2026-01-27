#pragma once

#include "../../src/wrapper/JavaClass/JavaClass.h"

#include "../../src/wrapper/sdk/java/util/UUID/UUID.h"
#include "../../src/wrapper/sdk/net/minecraft/util/ChatComponentText/ChatComponentText.h"

class Entity : public JavaClass
{
public:
	explicit Entity(const jobject instance);
	Entity(const char* name, const jobject instance);

	virtual ~Entity() override;

	virtual void Init() override;

	[[nodiscard]] virtual bool IsSprinting() const final;
	[[nodiscard]] virtual bool IsSneaking() const final;
	[[nodiscard]] virtual bool IsInvisible() const final;
	[[nodiscard]] virtual bool IsOnGround() const final;
	[[nodiscard]] virtual bool IsEating() const final;
	[[nodiscard]] virtual bool IsRiding() const final;

	[[nodiscard]] virtual float GetRotationYaw() const final;
	[[nodiscard]] virtual float GetRotationPitch() const final;
	[[nodiscard]] virtual float GetLastRotationYaw() const final;
	[[nodiscard]] virtual float GetLastRotationPitch() const final;

	[[nodiscard]] virtual double GetPosX() const final;
	[[nodiscard]] virtual double GetPosY() const final;
	[[nodiscard]] virtual double GetPosZ() const final;
	[[nodiscard]] virtual double GetLastTickPosX() const final;
	[[nodiscard]] virtual double GetLastTickPosY() const final;
	[[nodiscard]] virtual double GetLastTickPosZ() const final;
	[[nodiscard]] virtual double GetMotionX() const final;
	[[nodiscard]] virtual double GetMotionY() const final;
	[[nodiscard]] virtual double GetMotionZ() const final;

	[[nodiscard]] virtual std::unique_ptr<UUID_J> GetUniqueID() const final;

	[[nodiscard]] virtual std::string GetName() const final;

	virtual void AddChatMessage(const std::unique_ptr<IChatComponent>& chatComponent) const final;

private:
	inline static std::once_flag oflag{};

	inline static jmethodID isSprintingMethodID{ nullptr };
	inline static jmethodID isSneakingMethodID{ nullptr };
	inline static jmethodID isInvisibleMethodID{ nullptr };
	inline static jmethodID isEatingMethodID{ nullptr };
	inline static jmethodID isRidingMethodID{ nullptr };
	inline static jmethodID getUniqueIDMethodID{ nullptr };
	inline static jmethodID getNameMethodID{ nullptr };
	inline static jmethodID addChatMessageMethodID{ nullptr };

	inline static jfieldID onGroundFieldID{ nullptr };
	inline static jfieldID rotationYawFieldID{ nullptr };
	inline static jfieldID rotationPitchFieldID{ nullptr };
	inline static jfieldID prevRotationYawFieldID{ nullptr };
	inline static jfieldID prevRotationPitchFieldID{ nullptr };
	inline static jfieldID posXFieldID{ nullptr };
	inline static jfieldID posYFieldID{ nullptr };
	inline static jfieldID posZFieldID{ nullptr };
	inline static jfieldID lastTickPosXFieldID{ nullptr };
	inline static jfieldID lastTickPosYFieldID{ nullptr };
	inline static jfieldID lastTickPosZFieldID{ nullptr };
	inline static jfieldID motionXFieldID{ nullptr };
	inline static jfieldID motionYFieldID{ nullptr };
	inline static jfieldID motionZFieldID{ nullptr };
};