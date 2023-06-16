#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include"ViewProjection.h"
#include"Input.h"
#include"ImGuiManager.h"
#include"PlayerBullet.h"
#include"MathUtility.h"
#include<list>
class Player {
public:
	/// <summary>
	/// 
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle, Vector3& playerPosition);
	/// <summary>
	/// 
	/// </summary>
	void Update();
	/// <summary>
	/// 
	/// </summary>
	void Attack();
	/// <summary>
	/// 
	/// </summary>
	void Draw(ViewProjection& viewProjection);
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();

	Vector3 GetWorldRotation();

	void SetParent(const WorldTransform* parent);
	/// <summary>
	/// 
	/// </summary>
	~Player();
	/// <summary>
	/// 
	/// </summary>
	void OnCollision();
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	const std::list<PlayerBullet*>& GetBullets() { return bullets_; };
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	const float GetRadius() { return radius_; };

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	PlayerBullet* bullet_ = nullptr;
	std::list<PlayerBullet*> bullets_;
	const float radius_ = 1.0f;
};
