#include "TitleScene.h"
#include "MyMath.h"
#include <numbers>

using namespace KamataEngine;

//デストラクタ
TitleScene::~TitleScene() {
	delete model_;
	delete modelPlayer_;
}

void TitleScene::Initialize() { 
	model_ = Model::CreateFromOBJ("titleFont");
	modelPlayer_ = Model::CreateFromOBJ("player");

	camera_.Initialize();

	worldTransform_.Initialize();
	worldTransform_.scale_ = {2.0f, 2.0f, 2.0f};
	worldTransform_.translation_ = {0, 8, 0};

	worldTransformPlayer_.Initialize();
	worldTransformPlayer_.scale_ = {10,10,10};
	worldTransformPlayer_.translation_ = {0,-8,0};
	worldTransformPlayer_.rotation_.y = std::numbers::pi_v<float>;

}

void TitleScene::Update() {
//アフィン変換行列の生成
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();

	// 回転角度を更新
	rotate += 0.1f; // 回転速度を調整
	worldTransformPlayer_.rotation_.y = sin(rotate) + std::numbers::pi_v<float>; // Y軸を中心に回転

	worldTransformPlayer_.matWorld_ = MakeAffineMatrix(worldTransformPlayer_.scale_, worldTransformPlayer_.rotation_, worldTransformPlayer_.translation_);
	worldTransformPlayer_.TransferMatrix();

	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true; 
	}


}

void TitleScene::Draw() {
	// DirectXCommonのインスタンスを取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 描画開始
	Model::PreDraw(dxCommon->GetCommandList());

	model_->Draw(worldTransform_, camera_);

	modelPlayer_->Draw(worldTransformPlayer_, camera_);

    Model::PostDraw();

}
