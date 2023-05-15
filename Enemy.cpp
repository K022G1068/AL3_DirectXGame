#include "Enemy.h"

void Enemy::Initialize(Model* model, const Vector3& position) 
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	textureHandler_ = TextureManager::Load("white1x1.png");
}

void Enemy::Update() 
{ 
	worldTransform_.translation_.z -= 0.5f;
	worldTransform_.translation_.y += 0.1f;

	worldTransform_.UpdateMatrix(); 
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandler_);
}
