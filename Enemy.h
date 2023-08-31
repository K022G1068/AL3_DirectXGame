#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include<assert.h>
#include"EnemyBullet.h"
#include"Vector3.h"
#include"TimedCall.h"
#include"Collider.h"

enum class Phase {
	Approach,
	Leave,
};

class Enemy;
class Player;
class GameScene;
class BaseEnemyState {
public:
	virtual void Update(Enemy* pEnemy) = 0;
	virtual ~BaseEnemyState() = default;
};

class EnemyStateApproach : public BaseEnemyState {
public:
	void Update(Enemy* pEnemy);
	
	
};

class EnemyStateLeave : public BaseEnemyState {
public:
	void Update(Enemy* pEnemy);
};

class Enemy : public Collider{
public:
	/// <summary>
	/// 
	/// </summary>
	void Initialize(Model* model, const Vector3& position, const Vector3& scale, const float speed);
	Enemy();
	/// <summary>
	/// 
	/// </summary>
	~Enemy();
	/// <summary>
	/// 
	/// </summary>
	void Update();
	/// <summary>
	/// 
	/// </summary>
	void Draw(const ViewProjection& viewProjection);
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	Vector3 getPos() { return worldTransform_.translation_; };
	/// <summary>
	/// 
	/// </summary>
	/// <param name="pos"></param>
	void setPos(Vector3 pos) { worldTransform_.translation_ = pos; };
	/// <summary>
	/// 
	/// </summary>
	void ChangeState(BaseEnemyState* enemyState);
	/// <summary>
	/// 
	/// </summary>
	void Fire();
	/// <summary>
	/// 
	/// </summary>
	void ApproachInitialize();
	int32_t GetFireTimer() { return fireTimer; };
	void SetFireTimer(int32_t num) { fireTimer = num; };

	void SetPlayer(Player* player) { player_ = player; };
	bool IsDead() { return IsDead_; };
	Vector3 GetWorldPosition() override;
	/// <summary>
	/// 
	/// </summary>
	void OnCollision() override;
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	//const std::list<EnemyBullet*>& GetBullets() { return bullets_; };
	/// <summary>
	///
	/// </summary>
	/// <returns></returns>
	const float GetRadius() { return radius_; };
	/// <summary>
	/// 
	/// </summary>
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; };

	float GetSpeed() { return speed_; };

	void SetSpeed() { speed_ += 0.1f; };

	void Reset();

	void InitializeAttack();

	void ClearList() { timedCalls_.clear(); };

	void EnemyRespawn();

private:
	Phase phase_ = Phase::Approach;
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandler_ = 0;
	uint32_t redtext_ = 0;
	uint32_t whitetext_ = 0;

	float speed_ = 0.5f;

	BaseEnemyState* state_;
	EnemyBullet* bullet_ = nullptr;
	//std::unique_ptr<EnemyBullet> bullet_;
	
	int32_t fireTimer = 0;
	const float radius_ = 1.0f;
	Player* player_ = nullptr;
	GameScene* gameScene_ = nullptr;

	bool IsDead_;
	std::list<TimedCall*> timedCalls_;
	
	
public:	
	static const int kFireInterval = 120;
	int deathTimer_ = 300;
};
