#include "EnemyBullet.h"
#include "Player.h"


void EnemyBullet::Initialize(Model* model, const Vector3& pos, const Vector3& velocity) 
{ 
	assert(model);
	worldTransform_.Initialize();
	textureHandler_ = TextureManager::Load("sample.png");
	model_ = model;
	worldTransform_.translation_ = pos;
	worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 0.5f;
	worldTransform_.scale_.z = 3.0f;
	velocity_ = velocity;
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	float tangentLine = Length({
	    velocity_.x,
	    0.0f,
	    velocity_.z,
	});
	worldTransform_.rotation_.x = std::atan2(-velocity_.y, tangentLine);
	SetAttribute(kCollisionAttributeEnemy);
	SetMaskAttribute(kCollisionAttributePlayer);
}

void EnemyBullet::Update() 
{
	assert(player_);
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
	
	Vector3 toPlayer = player_->GetWorldPosition();
	Vector3 n_toPlayer = Normalize(toPlayer);
	Vector3 n_velocity = Normalize(velocity_);

	velocity_ = Lerp(n_velocity, n_toPlayer, 1.0f) * 0.5f;

	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	float tangentLine = Length({
	    velocity_.x,
	    0.0f,
	    velocity_.z,
	});
	worldTransform_.rotation_.x = std::atan2(-velocity_.y, tangentLine);

	
	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection_) 
{
	model_->Draw(worldTransform_, viewProjection_, textureHandler_);
}

Vector3 EnemyBullet::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}