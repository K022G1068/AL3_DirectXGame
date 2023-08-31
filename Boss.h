 #pragma once
#include"WorldTransform.h"
#include"Model.h"
#include<assert.h>
#include"EnemyBullet.h"
#include"Vector3.h"
#include"TimedCall.h"
#include"Collider.h"
#include"Input.h"

enum class BossPhase {
	Approach,
	Leave,
	Dash,
	Stay
};

class Boss;
class Player;
class GameScene;
class BaseBossState {
public:
	virtual void Update(Boss* boss) = 0;
	virtual ~BaseBossState() = default;
};

class BossStateApproach {
public:
	void Update(Boss* boss);
};

class BossStateLeave : public BaseBossState {
public:
	void Update(Boss* boss);
};
class BossStateStay : public BaseBossState {
public:
	void Update(Boss* boss);
};



class Boss : public Collider {
public:
	/// <summary>
	///
	/// </summary>
	void Initialize(Model* model, const Vector3& position);
	Boss();
	/// <summary>
	///
	/// </summary>
	~Boss();
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
	void ChangeState(BaseBossState* enemyState);
	/// <summary>
	///
	/// </summary>
	void Fire();
	void SetPlayer(Player* player) { player_ = player; };
	/// <summary>
	///
	/// </summary>
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; };
	Vector3 GetWorldPosition() override;
	void OnCollision() override;
	void Move();

private:
	BossPhase phase_ = BossPhase::Stay;
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandler_ = 0;
	uint32_t redtext_ = 0;
	uint32_t whitetext_ = 0;
	Input* input_ = nullptr;
	BaseBossState* state_;
	EnemyBullet* bullet_ = nullptr;
	// std::unique_ptr<EnemyBullet> bullet_;

	int32_t fireTimer = 0;
	const float radius_ = 1.0f;
	Player* player_ = nullptr;
	GameScene* gameScene_ = nullptr;

	bool IsDead_;
	std::list<TimedCall*> timedCalls_;
	TimedCall* HpCooldown_;
	int Hp_;

public:
	static const int kFireInterval = 60;
	int deathTimer_ = 300;
};
