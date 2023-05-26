#include "EnemyBullet.h"

void EnemyBullet::Initialize(Model* model, const Vector3& pos, const Vector3& velocity) 
{ 
	assert(model);
	worldTransform_.Initialize();
	textureHandler_ = TextureManager::Load("sample.png");
	model_ = model;
	worldTransform_.translation_ = pos;
	velocity_ = velocity;
}

void EnemyBullet::Update() 
{
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;
	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection_) 
{
	model_->Draw(worldTransform_, viewProjection_, textureHandler_);
}
