#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include"ViewProjection.h"
#include<assert.h>
#include"MathUtility.h"

class EnemyBullet {
public:
	/// <summary>
	/// 
	/// </summary>
	void Initialize(Model* model, const Vector3& pos, const Vector3& velocity);
	/// <summary>
	/// 
	/// </summary>
	void Update();
	/// <summary>
	/// 
	/// </summary>
	void Draw(const ViewProjection& viewProjection_);
	/// <summary>
	///
	/// </summary>
	/// <returns></returns>
	const float GetRadius() { return radius_; };
	Vector3 GetWorldPosition();
	/// <summary>
	///
	/// </summary>
	void OnCollision() { isDead_ = true; };
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool IsDead() { return isDead_; };
	
private:
	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	uint32_t textureHandler_ = 0;
	Vector3 velocity_;
	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;
	const float radius_ = 1.0f;
};
