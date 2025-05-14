#pragma once
#include <kamataEngine.h>
#include <vector>
#include "Player.h"
#include "Skydome.h"

class GameScene {
public:

	~GameScene();
	// 初期化

	std::vector <std::vector<KamataEngine::WorldTransform*>> WorldTransformBlocks_;

	bool isDebugCameraActive_ = false;

	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();

	KamataEngine::Sprite* sprite_ = nullptr;

	//uint32_t textureHandle_ = 0;	

	// モデル	
	KamataEngine::Model* model_ = nullptr;
	//ブロック
	KamataEngine::Model* modelBlock_ = nullptr;	
	//天球
	KamataEngine::Model* modelSkydome_ = nullptr;
	//ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;
	// カメラ
	KamataEngine::Camera camera_;

	// デバックカメラ
	KamataEngine::DebugCamera* debugCamera_ = nullptr;	


private:
	uint32_t textureHandle_ = 0;
	// プレイヤー
	Player* player_ = nullptr;
	Skydome* skydome_ = nullptr;
	
};