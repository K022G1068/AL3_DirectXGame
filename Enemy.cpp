#include "Enemy.h"

void Enemy::Initialize(Model* model, const Vector3& position) 
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	textureHandler_ = TextureManager::Load("white1x1.png");

	
}

Enemy::Enemy() 
{ 
	state_ = new EnemyStateApproach(); 
}

Enemy::~Enemy() 
{ 
	delete state_; 
}

void Enemy::Update() 
{ 
	state_->Update(this);
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

void EnemyStateApproach::Update(Enemy* pEnemy)
{
	Vector3 move = pEnemy->getPos();
	move.z -= 0.5f;
	pEnemy->setPos(move);
	if (pEnemy->getPos().z < -70.0f) {
		pEnemy->ChangeState(new EnemyStateLeave);
	}
}

void EnemyStateLeave::Update(Enemy* pEnemy) 
{
	Vector3 move = pEnemy->getPos();
	move.z += 0.5f;
	pEnemy->setPos(move);
	if (pEnemy->getPos().z > 70.0f) {
		pEnemy->ChangeState(new EnemyStateApproach);
	}
}
