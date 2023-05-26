#include "Enemy.h"
#include <MathUtility.h>

void Enemy::Initialize(Model* model, const Vector3& position) 
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	textureHandler_ = TextureManager::Load("white1x1.png");
	ApproachInitialize();
}

Enemy::Enemy() 
{ 
	state_ = new EnemyStateApproach(); 
}

Enemy::~Enemy() 
{ 
	delete state_; 
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
		bullet = nullptr;
	}
}

void Enemy::Update() 
{ 
	state_->Update(this);

	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	worldTransform_.UpdateMatrix(); 
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandler_);

	for (EnemyBullet* bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}

}

void Enemy::ChangeState(BaseEnemyState* enemyState) 
{ 
	//delete state_;
	state_ = enemyState;
}

void Enemy::Fire() 
{
	//bullet_ = new EnemyBullet;
	EnemyBullet* newBullet = new EnemyBullet();
	const float kBulletSpeed = -1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);
	bullets_.push_back(newBullet);
}

void Enemy::ApproachInitialize() 
{
	fireTimer = kFireInterval;
}

void EnemyStateApproach::Update(Enemy* pEnemy)
{
	Vector3 move = pEnemy->getPos();
	move.x += 0.1f;
	move.z -= 0.5f;
	pEnemy->SetFireTimer(pEnemy->GetFireTimer() - 1);
	pEnemy->setPos(move);
	if (pEnemy->getPos().z < -70.0f) {
		pEnemy->ChangeState(new EnemyStateLeave);
	}
	if (pEnemy->GetFireTimer() <= 0)
	{
		pEnemy->Fire();
		pEnemy->SetFireTimer(pEnemy->kFireInterval);
	}

	
}

void EnemyStateLeave::Update(Enemy* pEnemy) 
{
	Vector3 move = pEnemy->getPos();
	move.z += 0.5f;
	move.x -= 0.1f;
	pEnemy->setPos(move);
	if (pEnemy->getPos().z > 70.0f) {
		pEnemy->ChangeState(new EnemyStateApproach);
	}
}
