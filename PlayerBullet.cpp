#include "PlayerBullet.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position) { 
	assert(model);
	model_ = model;
	textureHandler_ = TextureManager::Load("white1x1.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}

void PlayerBullet::Update() { 
	worldTransform_.UpdateMatrix(); 
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandler_);
}
