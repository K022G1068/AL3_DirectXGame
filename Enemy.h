#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include<assert.h>
#include"EnemyBullet.h"
#include"Vector3.h"

enum class Phase {
	Approach,
	Leave,
};

class Enemy;

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

private:
	Phase phase_ = Phase::Approach;
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandler_ = 0;

	BaseEnemyState* state_;
	EnemyBullet* bullet_ = nullptr;
	//std::unique_ptr<EnemyBullet> bullet_;
	std::list<EnemyBullet*> bullets_;
	int32_t fireTimer = 0;

public:
	static const int kFireInterval = 60;
};
