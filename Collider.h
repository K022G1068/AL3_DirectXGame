#pragma once
#include"Vector3.h"
#include"CollisionConfig.h"
class Collider {
private:
	float radius_ = 1.0f;
	uint32_t collisionAttribute_ = 0xffffffff;
	uint32_t collisionMask_ = 0xffffffff;

public:
	float GetRadius() { return radius_; };
	void SetRadius(float radius) {radius_ = radius;};
	virtual void OnCollision();
	virtual Vector3 GetWorldPosition() = 0;
	uint32_t GetAttribute() { return collisionAttribute_; };
	void SetAttribute(uint32_t attribute) { collisionAttribute_ = attribute; };
	uint32_t GetMaskAttribute() { return collisionMask_; };
	void SetMaskAttribute(uint32_t attribute) { collisionMask_ = attribute; };
};
