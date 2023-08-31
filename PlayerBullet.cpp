#include "PlayerBullet.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) { 
	assert(model);
	model_ = model;
	redtext_ = TextureManager::Load("red1x1.png");
	whitetext_ = TextureManager::Load("white1x1.png");
	textureHandler_ = whitetext_;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	velocity_ = velocity;
	SetAttribute(kCollisionAttributePlayer);
	SetMaskAttribute(kCollisionAttributeEnemy);
}

void PlayerBullet::Update() { 

	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}

	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;



	Vector3 pos = GetWorldPosition();
	SetBox(pos, worldTransform_.scale_);

	ImGui::Begin("Bullet pos");
	ImGui::Text(
	    "A Min x = %f, y = %f z = %f", GetBox().min.x, GetBox().min.y, GetBox().min.z);
	ImGui::Text(
	    "A max x = %f, y = %f z = %f", GetBox().max.x,GetBox().max.y, GetBox().max.z);
	ImGui::End();
	worldTransform_.UpdateMatrix(); 
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandler_);
}

void PlayerBullet::OnCollision() {
	ImGui::Text("HITTTTT");
	isDead_ = true;
};

Vector3 PlayerBullet::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

