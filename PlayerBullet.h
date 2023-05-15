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

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandler_ = 0;
	Vector3 velocity_;
};
