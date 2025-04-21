#pragma once
#include <kamataEngine.h>
#include "Player.h"
class GameScene {
public:

	~GameScene();

	// 初期化

	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();

	KamataEngine::Sprite* sprite_ = nullptr;

	//uint32_t textureHandle_ = 0;	

	// モデル	
	KamataEngine::Model* model_ = nullptr;

	//ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;
	// カメラ
	KamataEngine::Camera camera_;


private:
	uint32_t textureHandle_ = 0;
	// プレイヤー
	Player* player_ = nullptr;
	
};