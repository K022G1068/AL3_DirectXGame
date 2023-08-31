#include "Player.h"
#include <assert.h>
#include"GameScene.h"

void Player::Initialize(Model* model, uint32_t textureHandle, Vector3& playerPosition) { 
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_.x += playerPosition.x;
	worldTransform_.translation_.y += playerPosition.y;
	worldTransform_.translation_.z += playerPosition.z;

	worldTransform3DReticle_.Initialize();
	uint32_t textureReticle = TextureManager::Load("target.png");
	sprite2DReticle_ = Sprite::Create(
	    textureReticle,
	    Vector2(640,360),
	    Vector4(1, 1, 1, 1), Vector2(0.5f, 0.5f));
	input_ = Input::GetInstance();
	SetAttribute(kCollisionAttributePlayer);
	SetMaskAttribute(kCollisionAttributeEnemy);
}

void Player::Update(ViewProjection& viewProjection) {
	

	//worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	//worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);
	//worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	//worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);

	//worldTransform_.translation_.x += move.x;
	//worldTransform_.translation_.y += move.y;
	//worldTransform_.translation_.z += move.z;

	

	//ImGui::SetNextWindowPos({10, 10});
	//ImGui::SetNextWindowSize({300, 100});
	//ImGui::Begin("Debug1");
	//float sliderValue[3] = {
	//    worldTransform_.translation_.x, worldTransform_.translation_.y,
	//    worldTransform_.translation_.z};
	//ImGui::SliderFloat3("PlayerPos", sliderValue, -20.0f, 20.0f);
	//worldTransform_.translation_ = {sliderValue[0], sliderValue[1], sliderValue[2]};

	//float sliderValue2[3] = {
	//    worldTransform_.scale_.x, worldTransform_.scale_.y, worldTransform_.scale_.z};
	//ImGui::SliderFloat3("PlayerScale", sliderValue2, -5.0f, 5.0f);
	//worldTransform_.scale_ = {sliderValue2[0], sliderValue2[1], sliderValue2[2]};

	////ImGui::Text("Min x = %f, y = %f z = %f", GetBox().min.x, GetBox().min.y, GetBox().min.z);
	////ImGui::Text("max x = %f, y = %f z = %f", GetBox().max.x, GetBox().max.y, GetBox().max.z);
	//ImGui::End();

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

	//3DReticle
	{	
		const float kDistancePlayerTo3DReticle = 50.0f;
		Vector3 offset = {0, 0, 1.0f};
		offset = TransformNormal(offset, worldTransform_.matWorld_);
		offset = Normalize(offset);
		offset.x *= kDistancePlayerTo3DReticle;
		offset.y *= kDistancePlayerTo3DReticle;
		offset.z *= kDistancePlayerTo3DReticle;
		worldTransform3DReticle_.translation_ = worldTransform_.translation_ + offset;
		worldTransform3DReticle_.UpdateMatrix();
	}

	//From 3D to 2D
	{ 
		Vector3 positionReticle = GetWorldPosition();

		Matrix4x4 matViewport =
		    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

		Matrix4x4 matViewProjectionViewport =
		    viewProjection.matView * viewProjection.matProjection * matViewport;

		positionReticle = Transform(positionReticle, matViewProjectionViewport);
		sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
	}

	//From 2D to 3D
	{ 
		GetCursorPos(&mousePos_);
		HWND hwnd = WinApp::GetInstance()->GetHwnd();
		ScreenToClient(hwnd, &mousePos_);

		sprite2DReticle_->SetPosition({
		    (float)mousePos_.x,
		    (float)mousePos_.y,
		});

		Matrix4x4 matViewport =
		    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

		Matrix4x4 matViewProjectionViewport =
		    viewProjection.matView * viewProjection.matProjection * matViewport;
		matViewProjectionViewport = Inverse(matViewProjectionViewport);

		Vector3 posNear = Vector3((float)mousePos_.x, (float)mousePos_.y, 0);
		Vector3 posFar = Vector3((float)mousePos_.x, (float)mousePos_.y, 1);

		posNear = Transform(posNear, matViewProjectionViewport);
		posFar = Transform(posFar, matViewProjectionViewport);

		Vector3 mouseDirection = posFar - posNear;
		mouseDirection = Normalize(mouseDirection);

		const float kDistanceTestObject = 50.0f;
		worldTransform3DReticle_.translation_ = posNear + mouseDirection * kDistanceTestObject;
		// Player mouse rotation
		{
			Vector3 rotation;
			rotation.x =
			    atan2f(worldTransform3DReticle_.translation_.y, worldTransform_.translation_.y);
			rotation.y =
			    atan2f(worldTransform3DReticle_.translation_.z, worldTransform_.translation_.z);
			// rotation.z = atan2f(worldTransform_.translation_.x,
			// worldTransform3DReticle_.translation_.x);

			worldTransform_.rotation_.x = rotation.x;
			worldTransform_.rotation_.y = rotation.y;
		}

		worldTransform3DReticle_.UpdateMatrix();
	}
	Move();
	Vector3 pos = GetWorldPosition();
	SetBox(pos, worldTransform_.scale_);

	//Bullet Cooldown
	shotCooldown_--;

	worldTransform_.UpdateMatrix();
}
void Player::Attack() 
{ 
	if (input_->TriggerKey(DIK_SPACE) && shotCooldown_ <= 0)
	{
		PlayerBullet* newBullet = new PlayerBullet();
		Vector3 velocity(0, 0, kBulletSpeed);

		velocity = GetWorldTransform3DReticle() - GetWorldPosition();
		velocity = Normalize(velocity) * kBulletSpeed;
		/*if (bullet_)
		{
			delete bullet_;
			bullet_ = nullptr;
		}*/
		newBullet->Initialize(model_, GetWorldPosition(), velocity);
		//bullet_ = newBullet;
		//bullets_.push_back(newBullet);
		gameScene_->AddPlayerBullet(newBullet);
		shotCooldown_ = 120;
		
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
	model_->Draw(worldTransform3DReticle_, viewProjection, textureHandle_);
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
	delete sprite2DReticle_;
}

void Player::OnCollision() { gameScene_->Die(); }

void Player::DrawUI() 
{ 
	sprite2DReticle_->Draw(); }

Vector3 Player::GetWorldTransform3DReticle() 
{ 
	Vector3 worldPos;
	worldPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
	worldPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
	worldPos.z = worldTransform3DReticle_.matWorld_.m[3][2];
	return worldPos; 
}

void Player::Move() 
{
	Vector3 move = {0, 0, 0};

	const float kCharacterSpeed = 0.2f;

	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;
	}

	if (input_->PushKey(DIK_SPACE)) {
		// move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_LSHIFT)) {
		move.y -= kCharacterSpeed;
	}

	if (input_->PushKey(DIK_S)) {
		move.y -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_W)) {
		move.y += kCharacterSpeed;
	}

	const float kRotSpeed = 0.02f;
	if (input_->PushKey(DIK_Q)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_E)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;
}
