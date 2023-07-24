#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include"ViewProjection.h"
#include"Input.h"
#include"ImGuiManager.h"
#include"PlayerBullet.h"
#include"MathUtility.h"
#include<list>
#include"Sprite.h"
#include"Collider.h"


class Player : public Collider{
public:
	/// <summary>
	/// 
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle, Vector3& playerPosition);
	/// <summary>
	/// 
	/// </summary>
	void Update(ViewProjection& viewProjection);
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
	Vector3 GetWorldPosition() override;

	Vector3 GetWorldRotation();

	void SetParent(const WorldTransform* parent);
	/// <summary>
	/// 
	/// </summary>
	~Player();
	/// <summary>
	/// 
	/// </summary>
	void OnCollision() override;
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
	/// <summary>
	/// 
	/// </summary>
	void DrawUI();

	Vector3 GetWorldTransform3DReticle();


private:
	WorldTransform worldTransform_;
	WorldTransform worldTransform3DReticle_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	PlayerBullet* bullet_ = nullptr;
	std::list<PlayerBullet*> bullets_;
	const float radius_ = 1.0f;
	//Reticle
	Sprite* sprite2DReticle_ = nullptr;
	POINT mousePos_ = {0, 0};

};
