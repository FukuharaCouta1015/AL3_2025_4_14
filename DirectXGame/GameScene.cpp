#include "GameScene.h"
#include "MyMath.h"

using namespace KamataEngine;

//デストラクタ
GameScene::~GameScene() {
	delete player_;
	delete model_; 
	delete debugCamera_;
	for (std::vector<WorldTransform*>& WorldTransformBlockLine : WorldTransformBlocks_) {
		for (WorldTransform* WorldTransformBlock : WorldTransformBlockLine) {
			delete WorldTransformBlock;
		}
		
	}
	WorldTransformBlocks_.clear();
}

void GameScene::Initialize() {
	//画像の初期化
	//textureHandle_ = TextureManager::Load("explosion.png");
	modelBlock_ = Model::CreateFromOBJ("cube");

	debugCamera_ = new DebugCamera(1280,720);

	const uint32_t kNumBlocksHorizontal = 20;
	const uint32_t kNumBlocksVertical = 10;

	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;

	WorldTransformBlocks_.resize(kNumBlocksVertical);
	for (uint32_t i = 0; i < kNumBlocksVertical; ++i) {
		WorldTransformBlocks_[i].resize(kNumBlocksHorizontal);
	}

	//キューブの生成
	for (uint32_t i = 0; i < kNumBlocksVertical; ++i) {

		for (uint32_t j = 0;j < kNumBlocksHorizontal; ++j) {
			if ((i+j) % 2 == 0) {
				continue;
			}
				WorldTransformBlocks_[i][j] = new WorldTransform();
				WorldTransformBlocks_[i][j]->Initialize();
				WorldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
				WorldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
			
		}
	}
	// ワールド
	worldTransform_.Initialize();


	// カメラ
	camera_.Initialize();

	model_ = Model::Create();

	player_ = new Player();
	player_->Initialize(model_, textureHandle_,&camera_);

	
}

void GameScene::Update() {

	debugCamera_->Update();

	#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif // _DEBUG

	//カメラの処理
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		camera_.TransferMatrix();
	} else {
		camera_.UpdateMatrix();
	}

	player_->Update();
	for (std::vector<WorldTransform*>& WorldTransformBlockLine : WorldTransformBlocks_) {
		for (WorldTransform* WorldTransformBlock : WorldTransformBlockLine) {
			if (!WorldTransformBlock) {
				continue;
			}
			WorldTransformBlock->matWorld_ = MakeAffineMatrix(WorldTransformBlock->scale_, WorldTransformBlock->rotation_, WorldTransformBlock->translation_);
			WorldTransformBlock->TransferMatrix();
		}

	}

}


void GameScene::Draw() {

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	// 描画開始
	Model::PreDraw(dxCommon->GetCommandList());
	for (std::vector<WorldTransform*>& WorldTransformBlockLine : WorldTransformBlocks_) {
		for (WorldTransform* WorldTransformBlock : WorldTransformBlockLine) {
			if (!WorldTransformBlock) {
				continue;
			}
			modelBlock_->Draw(*WorldTransformBlock, camera_);
		}
	}
	
	//model_->Draw(worldTransform_,camera_,textureHandle_);
	player_->Draw();
	Model::PostDraw();


}
