#include "GameScene.h"
#include "MyMath.h"
#include <map>
#include "MapChipField.h"

using namespace KamataEngine;

//デストラクタ
GameScene::~GameScene() {
	delete player_;
	delete model_; 
	delete debugCamera_;
	delete modelBlock_;
	delete modelSkydome_;
	delete skydome_;
	delete mapChipField_;
	delete cameraController_;


	for (std::vector<WorldTransform*>& WorldTransformBlockLine : WorldTransformBlocks_) {
		for (WorldTransform* WorldTransformBlock : WorldTransformBlockLine) {
			delete WorldTransformBlock;
		}
		
	}
	WorldTransformBlocks_.clear();
}

void GameScene::Initialize() {
	//画像の初期化
	model_ = Model::CreateFromOBJ("player");
	modelBlock_ = Model::CreateFromOBJ("block");

	modelSkydome_ = Model::CreateFromOBJ("skydome",true);

	debugCamera_ = new DebugCamera(1280,720);


	mapChipField_ = new MapChipField();
	mapChipField_->LodeMapChipCsv("Resources/blocks.csv");
	


	//const uint32_t kNumBlocksHorizontal = 20;
	//const uint32_t kNumBlocksVertical = 10;

	//const float kBlockWidth = 2.0f;
	//const float kBlockHeight = 2.0f;

	GenerateBlocks();

	//WorldTransformBlocks_.resize(kNumBlocksVertical);
	//for (uint32_t i = 0; i < kNumBlocksVertical; ++i) {
	//	WorldTransformBlocks_[i].resize(kNumBlocksHorizontal);
	//}

	
	// ワールド
	worldTransform_.Initialize();


	// カメラ
	camera_.Initialize();

	

	player_ = new Player();
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1, 18);
	player_->Initialize(model_, &camera_, playerPosition);


	skydome_= new Skydome();
	skydome_->Initialize(modelSkydome_,&camera_);

	cameraController_ = new CameraController();
	cameraController_->Initialize();
	cameraController_->SetTarget(player_);
	cameraController_->Reset();



	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};

	cameraController_->SetMovebleArea(cameraArea);

	player_->SetMapChipField(mapChipField_);
}

void GameScene::Update() {

	debugCamera_->Update();
	cameraController_->Update();

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
		camera_.matView = cameraController_->GetViewProjection().matView;
		camera_.matProjection = cameraController_->GetViewProjection().matProjection;
		camera_.TransferMatrix();
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

void GameScene::GenerateBlocks() {

	uint32_t numBlocksHorizontal = mapChipField_->GetNumBlockHorizontal();
	uint32_t numBlocksVertical = mapChipField_->GetNumBlockVertical();

	WorldTransformBlocks_.resize(numBlocksVertical);
	for (uint32_t i = 0; i < numBlocksVertical; ++i) {
		WorldTransformBlocks_[i].resize(numBlocksHorizontal);
	}

	// キューブの生成
	for (uint32_t i = 0; i < numBlocksVertical; ++i) {

		for (uint32_t j = 0; j < numBlocksHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				WorldTransformBlocks_[i][j] = worldTransform;
				WorldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::Draw() {

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	

	// 描画開始
	Model::PreDraw(dxCommon->GetCommandList());
	skydome_->Draw();

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


