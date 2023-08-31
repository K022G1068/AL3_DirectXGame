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
	delete modelSkydome_;
	delete skydome_;
	delete railCamera_;

	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
		bullet = nullptr;
	}

	for (Enemy* enemy : enemies_) {
		delete enemy;
		enemy = nullptr;
	}
	delete boss_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	textureHandle_ = TextureManager::Load("sample.png");
	titleTextureHandle_ = TextureManager::Load("title.png");
	sprite_ = Sprite::Create(textureHandle_, {50, 50});
	titleScene_ = Sprite::Create(titleTextureHandle_, {0, 0});
	model_ = Model::Create();
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	LoadEnemyPopData();
	//Player Component
	player_ = new Player();
	boss_ = new Boss();
	Vector3 playerPosition(0, -5.0f, 20.0f);
	Vector3 railPosition(0, 0, -50.0f);
	player_->Initialize(model_, textureHandle_, playerPosition);
	player_->SetGameScene(this);
	boss_->Initialize(model_, {0, 5.0f, 20.0f});
	boss_->SetPlayer(player_);
	boss_->SetGameScene(this);
	
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	skydome_ = new Skydome();
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_->Initialize(modelSkydome_);
	
	//Enemy Component
	//AddEnemy({0, 3, 0}); 

	
	//Rail camera
	railCamera_ = new RailCamera();
	railCamera_->Initialize(railPosition, {0,0,0}, viewProjection_);
	player_->SetParent(&railCamera_->GetWorldTransform());
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

}

void GameScene::Update() {

	if (scene_ == SCENE::Title)
	{
		if (input_->PushKey(DIK_SPACE))
		{
			scene_ = SCENE::Play;
		}
	}

	if (scene_ == SCENE::Play)
	{
		player_->Update(viewProjection_);
		for (Enemy* enemy : enemies_) {
			if (enemy) {
				enemy->Update();
			}
		}

		boss_->Update();

#ifdef _DEBUG
		if (input_->TriggerKey(DIK_R) && isDebugCameraActive_ == false) {
			isDebugCameraActive_ = true;
		} else if (isDebugCameraActive_ && input_->TriggerKey(DIK_R)) {
			isDebugCameraActive_ = false;
		}
#endif // _DEBUG

		if (isDebugCameraActive_) {
			debugCamera_->Update();
			viewProjection_.matView = debugCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
			viewProjection_.TransferMatrix();

		} else {
			// viewProjection_.UpdateMatrix();
			railCamera_->Update();
			viewProjection_.matView = railCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
			viewProjection_.TransferMatrix();
		}
		skydome_->Update();

		

		UpdateEnemyPopCommands();
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

		for (PlayerBullet* bullet : playerBullets_) {
			bullet->Update();
		}

		playerBullets_.remove_if([](PlayerBullet* bullet) {
			if (bullet->IsDead()) {
				delete bullet;
				return true;
			}
			return false;
		});

		enemies_.remove_if([](Enemy* enemy) {
			if (enemy->IsDead()) {
				delete enemy;
				return true;
			}
			return false;
		});
		CheckAllCollisions();


	}
	

	//Debug IMGUI
	//
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

	if (scene_ == SCENE::Play) {
		player_->Draw(viewProjection_);
		for (Enemy* enemy : enemies_) {
			enemy->Draw(viewProjection_);
		}

		for (EnemyBullet* bullet : bullets_) {
			bullet->Draw(viewProjection_);
		}
		for (PlayerBullet* bullet : playerBullets_) {
			bullet->Draw(viewProjection_);
		}

		boss_->Draw(viewProjection_);

		skydome_->Draw(viewProjection_);
		railCamera_->DrawSpline();
	}
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
	if (scene_ == SCENE::Play) {
		player_->DrawUI();
	} else if (scene_ == SCENE::Title) {
		titleScene_->Draw();
	}
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() 
{

	//const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

	std::list<Collider*> colliders_;
	colliders_.push_back(player_);
	colliders_.push_back(boss_);
	for (Enemy* enemy : enemies_) {
		colliders_.push_back(enemy);
	}
	for (EnemyBullet* bullet : bullets_) {
		colliders_.push_back(bullet);
	}
	for (PlayerBullet* p_bullet : playerBullets_) {
		colliders_.push_back(p_bullet);
	}

	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA)
	{
		Collider* A = *itrA;

		//Iterator B
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB)
		{
			Collider* B = *itrB;
			if (A->GetAttribute() != B->GetMaskAttribute() ||
				A->GetMaskAttribute() != B->GetAttribute())
			{
				return;
			}
			/*ImGui::Text(
			    "A Min x = %f, y = %f z = %f", A->GetBox().min.x, A->GetBox().min.y,
			    A->GetBox().min.z);
			ImGui::Text(
			    "A max x = %f, y = %f z = %f", A->GetBox().max.x, A->GetBox().max.y,
			    A->GetBox().max.z);

			ImGui::Text(
			    "B Min x = %f, y = %f z = %f", B->GetBox().min.x, B->GetBox().min.y,
			    B->GetBox().min.z);
			ImGui::Text(
			    "B max x = %f, y = %f z = %f", B->GetBox().max.x, B->GetBox().max.y,
			    B->GetBox().max.z);*/
			CheckCollisionPair(A, B);
			if (AABBCollisionPair(A, B)) {
				A->OnCollision();
				B->OnCollision();
			}
		}
	}
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) 
{ 
	enemyBullet->SetPlayer(player_);
	bullets_.push_back(enemyBullet);
}

void GameScene::AddPlayerBullet(PlayerBullet* playerBullet) 
{
	playerBullets_.push_back(playerBullet);
}

void GameScene::AddEnemy(Vector3 pos, Vector3 scale) { 
	Enemy* obj = new Enemy();
	obj->Initialize(model_, pos, scale, enemySpeed_);
	obj->SetPlayer(player_);
	obj->SetGameScene(this);
	obj->InitializeAttack();

	enemies_.push_back(obj);
}

void GameScene::LoadEnemyPopData() 
{ 
	std::ifstream file;
	file.open("./Resources/enemyPop.csv");

	enemyPopCommands << file.rdbuf();

	file.close();
}

void GameScene::UpdateEnemyPopCommands() 
{ 
	if (wait) {
		waitTimer--;
		if (waitTimer <= 0) {
			wait = false;
		}

		return;
	}

	std::string line;

	while (getline(enemyPopCommands, line))
	{
		std::istringstream line_stream(line);

		std::string word;

		getline(line_stream, word, ',');

		if (word.find("//") == 0) {
			continue;
		}

		if (word.find("POP") == 0)
		{
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float s_x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float s_y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float s_z = (float)std::atof(word.c_str());

			AddEnemy(Vector3(x, y, z), Vector3(s_x, s_y, s_z));
		}
		else if (word.find("WAIT") == 0)
		{
			getline(line_stream, word, ',');
			int32_t waitTime = std::atoi(word.c_str());

			wait = true;
			waitTimer = waitTime;

			break;	

		}
		else if (word.find("SPEED") == 0)
		{
			enemySpeed_ += 0.1f;
		}

		if (enemyPopCommands.peek() == EOF)
		{
			enemyPopCommands.clear();
			enemyPopCommands.seekg(0);
		}

	}
	
}

void GameScene::Die() 
{ 
	scene_ = SCENE::Title;
	enemies_.clear();
	bullets_.clear();
	playerBullets_.clear();
	enemyPopCommands.seekg(0);
	Vector3 playerPosition(0, -5.0f, 20.0f);
	player_->SetPosition(playerPosition);
}

void GameScene::CheckCollisionPair(Collider* colliderA, Collider* colliderB) 
{	
	Vector3 posA, posB;
	posA = colliderA->GetWorldPosition();
	posB = colliderB->GetWorldPosition();
	float length = sqrtf(
	    powf(posA.x - posB.x, 2.0f) + powf(posA.y - posB.y, 2.0f) + powf(posA.z - posB.z, 2.0f));

	if (length < colliderA->GetRadius() + colliderB->GetRadius())
	{
		colliderA->OnCollision();
		colliderB->OnCollision();
	}	

}

bool GameScene::AABBCollisionPair(Collider* colliderA, Collider* colliderB) 
{
	return (colliderA->GetBox().min.x <= colliderB->GetBox().max.x &&
	        colliderA->GetBox().max.x >= colliderB->GetBox().min.x) &&
	       (colliderA->GetBox().min.y <= colliderB->GetBox().max.y &&
	        colliderA->GetBox().max.y >= colliderB->GetBox().min.y) &&
	       (colliderA->GetBox().min.z <= colliderB->GetBox().max.z &&
	        colliderA->GetBox().max.z >= colliderB->GetBox().min.z);
	
}

