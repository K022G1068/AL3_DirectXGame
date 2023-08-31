#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "DebugCamera.h"
#include "Enemy.h"
#include "Skydome.h"
#include "RailCamera.h"
#include "EnemyBullet.h"
#include <sstream>
#include <fstream>
#include "Collider.h"
#include "Boss.h"

/// <summary>
/// ゲームシーン
/// </summary>
/// 
enum SCENE {
	Title,
	Play,
};
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	/// <summary>
	/// 
	/// </summary>
	void CheckAllCollisions();
	/// <summary>
	/// 
	/// </summary>
	/// <param name="enemyBullet"></param>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	void AddPlayerBullet(PlayerBullet* playerBullet);

	void AddEnemy(Vector3 pos, Vector3 scale);

	//Reading CSV
	void LoadEnemyPopData();

	//Update enemy state
	void UpdateEnemyPopCommands();

	void Die();

	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

	bool AABBCollisionPair(Collider* colliderA, Collider* colliderB);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	uint32_t textureHandle_ = 0;
	uint32_t titleTextureHandle_ = 0;
	Sprite* sprite_ = nullptr;
	Sprite* titleScene_ = nullptr;
	Model* model_ = nullptr;
	Model* modelSkydome_ = nullptr;
	Skydome* skydome_ = nullptr;
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	bool isDebugCameraActive_ = false;
	DebugCamera* debugCamera_ = nullptr;

	Player* player_ = nullptr;
	Boss* boss_ = nullptr;

	std::list<Enemy*> enemies_;
	std::list<EnemyBullet*> bullets_;
	std::list<PlayerBullet*> playerBullets_;
	RailCamera* railCamera_ = nullptr;

	std::stringstream enemyPopCommands;

	bool wait = false;
	int32_t waitTimer = 0;
	int scene_ = SCENE::Title;

	float enemySpeed_ = 0.5f;
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
