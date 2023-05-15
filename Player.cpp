#include "Player.h"
#include <assert.h>

void Player::Initialize(Model* model, uint32_t textureHandle) 
{ 
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	input_ = Input::GetInstance();
}

void Player::Update() 
{ 
	Vector3 move = {0, 0, 0};

	const float kCharacterSpeed = 0.2f;

	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;
	}

	if (input_->PushKey(DIK_S)) {
		move.y -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_W)) {
		move.y += kCharacterSpeed;
	}

	const float kRotSpeed = 0.02f;
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.x += kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.x -= kRotSpeed;
	}

	const float kMoveLimitX = 20.0f;
	const float kMoveLimitY = 20.0f;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

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
	if (bullet_)
	{
		bullet_->Update();
	}

	for (PlayerBullet* bullet : bullets_ ) {
		bullet->Update();
	}

}
void Player::Attack() 
{ 
	if (input_->TriggerKey(DIK_SPACE))
	{
		PlayerBullet* newBullet = new PlayerBullet();
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
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
