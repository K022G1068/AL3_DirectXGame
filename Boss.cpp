#include "Boss.h"
#include "Player.h"
#include "GameScene.h"
#include <MathUtility.h>

void Boss::Initialize(Model* model, const Vector3& position) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = {3.0f, 3.0f, 3.0f};
	redtext_ = TextureManager::Load("red1x1.png");
	whitetext_ = TextureManager::Load("white1x1.png");
	textureHandler_ = whitetext_;
	// ApproachInitialize();
	input_ = Input::GetInstance();
	SetAttribute(kCollisionAttributeEnemy);
	SetMaskAttribute(kCollisionAttributePlayer);
}

Boss::Boss() { state_ = new BossStateStay(); }

Boss::~Boss() {
	delete state_;
	for (TimedCall* time : timedCalls_) {
		delete time;
		// time = nullptr;
	}
}

void Boss::Update()
{
	textureHandler_ = whitetext_;
	state_->Update(this);
	if (Hp_ <= 0) {
		IsDead_ = true;
	}

	timedCalls_.remove_if([](TimedCall* time) {
		if (time->IsFinished()) {
			delete time;
			return true;
		}
		return false;
	});

	for (TimedCall* time : timedCalls_) {
		time->Update();
	}
	/*ImGui::Text(
			"A Min x = %f, y = %f z = %f", GetBox().min.x,GetBox().min.y,
			GetBox().min.z);
		ImGui::Text(
			"A max x = %f, y = %f z = %f", GetBox().max.x, GetBox().max.y,
			GetBox().max.z);*/
	// Reset();
	Move();
	Vector3 pos = GetWorldPosition();
	SetBox(pos, worldTransform_.scale_);
	worldTransform_.UpdateMatrix();
}

void Boss::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandler_);
}

void Boss::ChangeState(BaseBossState* bossState) {
	// delete state_;
	state_ = bossState;
}

void Boss::Fire() {}

Vector3 Boss::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	

	return worldPos;
}

void Boss::OnCollision() { textureHandler_ = redtext_; }

//void BossStateApproach::Update(Boss* boss) {
	//Vector3 move = pEnemy->getPos();
	//move.x += 0.1f;
	//move.z -= 0.5f;
	//// pEnemy->SetFireTimer(pEnemy->GetFireTimer() - 1);
	//pEnemy->setPos(move);
	//if (pEnemy->getPos().z < -70.0f) {
	//	pEnemy->ClearList();
	//	pEnemy->ChangeState(new EnemyStateLeave);
	//}
//}

//void BossStateLeave::Update(Boss* boss) {
	//Vector3 move = pEnemy->getPos();
	//move.z += 0.5f;
	//move.x -= 0.1f;
	//pEnemy->setPos(move);
	//if (pEnemy->getPos().z > 70.0f) {
	//	pEnemy->ChangeState(new EnemyStateApproach);
	//}
	//pEnemy->deathTimer_--;
//}

void BossStateStay::Update(Boss* boss) {
	//Just stay and shoot
	Vector3 move;
	move.x = 0;

	boss->Fire();
}

void Boss::Move() {
	Vector3 move = {0, 0, 0};

	const float kCharacterSpeed = 0.2f;

	if (input_->PushKey(DIK_J)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_L)) {
		move.x += kCharacterSpeed;
	}

	if (input_->PushKey(DIK_SPACE)) {
		// move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_LSHIFT)) {
		move.y -= kCharacterSpeed;
	}

	if (input_->PushKey(DIK_I)) {
		move.z -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_K)) {
		move.z += kCharacterSpeed;
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

//void BossStateDash

