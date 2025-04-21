#include "GameScene.h"
using namespace KamataEngine;

GameScene::~GameScene() {
	delete player_;
	delete model_; 
}

void GameScene::Initialize() {
	//画像の初期化
	textureHandle_ = TextureManager::Load("explosion.png");
	// ワールド
	worldTransform_.Initialize();


	// カメラ
	camera_.Initialize();

	model_ = Model::Create();

	player_ = new Player();
	player_->Initialize(model_, textureHandle_,&camera_);
}

void GameScene::Update() {
	player_->Update();
}

void GameScene::Draw() {

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	// 描画開始
	Model::PreDraw(dxCommon->GetCommandList());

	//model_->Draw(worldTransform_,camera_,textureHandle_);
	player_->Draw();
	Model::PostDraw();


}
