#pragma once
#include "kamataEngine.h"


class Player {

public:
	void Initialize(KamataEngine::Model* model_, uint32_t textureHandle, KamataEngine::Camera* camera);
	// 更新
	void Update();
	// 描画
	void Draw();



private:

	// モデル
	KamataEngine::Model* model_ = nullptr;

	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;
	// カメラ
	KamataEngine::Camera* camera_;
	uint32_t textureHandle_ = 0u;
	


};
