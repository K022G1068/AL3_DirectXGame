#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include<assert.h>

class Enemy {
public:
	/// <summary>
	/// 
	/// </summary>
	void Initialize(Model* model, const Vector3& position);
	/// <summary>
	/// 
	/// </summary>
	void Update();
	/// <summary>
	/// 
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandler_ = 0;

};
