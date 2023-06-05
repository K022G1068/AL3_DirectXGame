#pragma once
#include"Model.h"
#include"Vector3.h"
#include"ViewProjection.h"
#include"WorldTransform.h"
#include<cassert>
class PlayerBullet {
public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="model"></param>
	/// <param name="position"></param>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// Update
	/// </summary>
	void Update();

	/// <summary>
	/// Draw
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool IsDead() const { return isDead_; };
	/// <summary>
	/// 
	/// </summary>
	void OnCollision() { isDead_ = true; };
	/// <summary>
	///
	/// </summary>
	/// <returns></returns>
	const float GetRadius() { return radius_; };
	Vector3 GetWorldPosition();
	

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandler_ = 0;
	Vector3 velocity_;
	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;
	const float radius_ = 1.0f;
};
