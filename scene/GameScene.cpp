#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete sprite_; 
	delete model_;
	delete player_;
	delete debugCamera_;
	delete enemy_;
	delete modelSkydome_;
	delete skydome_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	textureHandle_ = TextureManager::Load("sample.png");
	sprite_ = Sprite::Create(textureHandle_, {50, 50});
	model_ = Model::Create();
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	player_ = new Player();
	player_->Initialize(model_, textureHandle_);
	enemy_ = new Enemy();
	enemy_->Initialize(model_, {0,3,50});
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	skydome_ = new Skydome();
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_->Initialize(modelSkydome_);
	enemy_->SetPlayer(player_);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {
	player_->Update();
	if (enemy_) {
		enemy_->Update();
	}
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_R)) {
		isDebugCameraActive_ = true;
	}
#endif // _DEBUG

	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
	}
	skydome_->Update();
	CheckAllCollisions();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	
#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	player_->Draw(viewProjection_);
	if (enemy_) {
		enemy_->Draw(viewProjection_);
	}

	skydome_->Draw(viewProjection_);
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() 
{
	Vector3 posA, posB;
	
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

	#pragma region player and enemy bullet
	posA = player_->GetWorldPosition();
	for (EnemyBullet* bullet : enemyBullets)
	{
		posB = bullet->GetWorldPosition();
		float length = sqrtf(powf(posA.x - posB.x, 2.0f) + powf(posA.y - posB.y, 2.0f) + powf(posA.z - posB.z, 2.0f));
		if (length <= player_->GetRadius() + bullet->GetRadius())
		{
			bullet->OnCollision();
			//player_->OnCollision();
		}
	}
	#pragma endregion
	
	#pragma region player bullet and enemy
	posA = enemy_->GetWorldPosition();
	for (PlayerBullet* bullet : playerBullets) {
		posB = bullet->GetWorldPosition();
		float length = sqrtf(
		    powf(posA.x - posB.x, 2.0f) + powf(posA.y - posB.y, 2.0f) +
		    powf(posA.z - posB.z, 2.0f));
		if (length <= enemy_->GetRadius() + bullet->GetRadius()) {
			bullet->OnCollision();
			//enemy_->OnCollision();
		}
	}
	#pragma endregion
	
	#pragma region player bullet and enemy bullet
	for (EnemyBullet* e_bullet : enemyBullets) {
		posB = e_bullet->GetWorldPosition();
		for (PlayerBullet* p_bullet : playerBullets) {
			posA = p_bullet->GetWorldPosition();
			float length = sqrtf(
			    powf(posA.x - posB.x, 2.0f) + powf(posA.y - posB.y, 2.0f) +
			    powf(posA.z - posB.z, 2.0f));
			if (length <= e_bullet->GetRadius() + p_bullet->GetRadius()) {
				e_bullet->OnCollision();
				p_bullet->OnCollision();
			}
		}
		
	}
	#pragma endregion
}
