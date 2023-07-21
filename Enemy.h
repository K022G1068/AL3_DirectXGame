#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include<assert.h>
#include"EnemyBullet.h"
#include"Vector3.h"
#include"TimedCall.h"


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

class Enemy {
public:
	/// <summary>
	/// 
	/// </summary>
	void Initialize(Model* model, const Vector3& position);
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
	Vector3 GetWorldPosition();
	/// <summary>
	/// 
	/// </summary>
	void OnCollision();
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

	void Reset();

	void InitializeAttack();

	void ClearList() { timedCalls_.clear(); };

private:
	Phase phase_ = Phase::Approach;
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandler_ = 0;

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
	static const int kFireInterval = 60;
	int deathTimer_ = 300;
};
