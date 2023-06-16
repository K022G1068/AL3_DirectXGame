#include "Player.h"
#include <assert.h>

void Player::Initialize(Model* model, uint32_t textureHandle, Vector3& playerPosition) { 
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_.x += playerPosition.x;
	worldTransform_.translation_.y += playerPosition.y;
	worldTransform_.translation_.z += playerPosition.z;
	input_ = Input::GetInstance();
}

void Player::Update() {
	

	//worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	//worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);
	//worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	//worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);

	//worldTransform_.translation_.x += move.x;
	//worldTransform_.translation_.y += move.y;
	//worldTransform_.translation_.z += move.z;

	worldTransform_.UpdateMatrix();

	ImGui::SetNextWindowPos({10, 10});
	ImGui::SetNextWindowSize({300, 100});
	ImGui::Begin("Debug1");
	float sliderValue[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	ImGui::SliderFloat3("PlayerPos", sliderValue, -20.0f, 20.0f);
	worldTransform_.translation_ = {sliderValue[0], sliderValue[1], sliderValue[2]};
	ImGui::End();

	Attack();
	if (bullet_) {
		bullet_->Update();
	}

	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
}
void Player::Attack() 
{ 
	if (input_->TriggerKey(DIK_SPACE))
	{
		PlayerBullet* newBullet = new PlayerBullet();
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		velocity = TransformNormal(velocity, worldTransform_.matWorld_);
		/*if (bullet_)
		{
			delete bullet_;
			bullet_ = nullptr;
		}*/
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);
		//bullet_ = newBullet;
		bullets_.push_back(newBullet);
	}

}
void Player::Draw(ViewProjection& viewProjection) 
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	/*if (bullet_) {
		bullet_->Draw(viewProjection);
	}*/
	for (PlayerBullet* bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
}

Vector3 Player::GetWorldPosition() { 
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	
	return worldPos; 
}

Vector3 Player::GetWorldRotation() 
{
	Vector3 worldRot;
	worldRot.x = worldTransform_.rotation_.x;
	worldRot.y = worldTransform_.rotation_.y;
	worldRot.z = worldTransform_.rotation_.z;

	return worldRot;
}

void Player::SetParent(const WorldTransform* parent) 
{ 
	worldTransform_.parent_ = parent; 
}

Player::~Player() { 
	/*if (bullet_) {
		delete bullet_;
		bullet_ = nullptr;
	}*/
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
		bullet = nullptr;
	}
	
}
