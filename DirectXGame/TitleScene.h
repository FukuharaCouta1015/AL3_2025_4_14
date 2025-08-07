#pragma once
#include "MyMath.h"
#include "kamataEngine.h"
#include <vector>

class TitleScene {

	
public:
	// デストラクタ
	~TitleScene();
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();

	bool IsFinished() const { return finished_; }


	private:
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Model* modelPlayer_ = nullptr;
	KamataEngine::Camera camera_ ;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::WorldTransform worldTransformPlayer_;
	float rotate = 0.0f;    // 回転角度
	bool finished_ = false; // タイトルシーンが終了したかどうか

};