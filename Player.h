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
	void Initialize(Model* model, uint32_t textureHandle);
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

	

	~Player();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	PlayerBullet* bullet_ = nullptr;
	std::list<PlayerBullet*> bullets_;
};
