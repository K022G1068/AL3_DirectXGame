#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "ViewProjection.h"
class Skydome 
{
public:
	~Skydome();
	/// <summary>
	/// 
	/// </summary>
	void Initialize(Model* model);
	/// <summary>
	/// 
	/// </summary>
	void Update();
	/// <summary>
	/// 
	/// </summary>
	void Draw(ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
};
