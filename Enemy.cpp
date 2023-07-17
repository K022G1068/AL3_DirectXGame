#include "Enemy.h"
#include "Player.h"
#include"GameScene.h"
#include <MathUtility.h>

void Enemy::Initialize(Model* model, const Vector3& position) 
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	textureHandler_ = TextureManager::Load("white1x1.png");
	//ApproachInitialize();
}

Enemy::Enemy() 
{ 
	state_ = new EnemyStateApproach(); 
}

Enemy::~Enemy() 
{ 
	delete state_; 
	for (TimedCall* time : timedCalls_) {
		delete time;
		//time = nullptr;
	}
}

void Enemy::Update() 
{ 
	state_->Update(this);
	if (--deathTimer_ <= 0) {
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

	//Reset();

	worldTransform_.UpdateMatrix(); 
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandler_);

	
}

void Enemy::ChangeState(BaseEnemyState* enemyState) 
{ 
	//delete state_;
	state_ = enemyState;
}

void Enemy::Fire() 
{
	assert(player_);
	Vector3 distance;
	distance.x = GetWorldPosition().x - player_->GetWorldPosition().x;
	distance.y = GetWorldPosition().y - player_->GetWorldPosition().y;
	distance.z = GetWorldPosition().z - player_->GetWorldPosition().z;

	float lenght = sqrtf(powf(distance.x, 2.f) + powf(distance.y,2.f) + powf(distance.z,2.f));

	distance.x /= lenght; 
	distance.y /= lenght; 
	distance.z /= lenght; 

	const float kBulletSpeed = -1.0f;
	Vector3 velocity(distance.x * kBulletSpeed, distance.y * kBulletSpeed, distance.z * kBulletSpeed);


	EnemyBullet* newBullet = new EnemyBullet();
	
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);
	gameScene_->AddEnemyBullet(newBullet);
}

void Enemy::ApproachInitialize() {
	Reset();
}

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}


void EnemyStateApproach::Update(Enemy* pEnemy)
{
	Vector3 move = pEnemy->getPos();
	move.x += 0.1f;
	move.z -= 0.5f;
	//pEnemy->SetFireTimer(pEnemy->GetFireTimer() - 1);
	pEnemy->setPos(move);
	if (pEnemy->getPos().z < -70.0f) {
		pEnemy->ClearList();
		pEnemy->ChangeState(new EnemyStateLeave);
	}
	
}

void Enemy::Reset() {
	Fire();
	timedCalls_.push_back(new TimedCall(std::bind(&Enemy::Reset, this), kFireInterval));
}

void Enemy::InitializeAttack() 
{ ApproachInitialize(); }

void EnemyStateLeave::Update(Enemy* pEnemy) 
{
	Vector3 move = pEnemy->getPos();
	move.z += 0.5f;
	move.x -= 0.1f;
	pEnemy->setPos(move);
	if (pEnemy->getPos().z > 70.0f) {
		pEnemy->ChangeState(new EnemyStateApproach);
		
	}
	pEnemy->deathTimer_--;
}
