#define NOMINMAX
#include "Player.h"
#include "MapChipField.h"

#include "MyMath.h"
#include "kamataEngine.h"
#include <algorithm>
#include <cassert>
#include <numbers>
using namespace KamataEngine;
using namespace MathUtility;

void Player::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) {

	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	camera_ = camera;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

void Player::Update() {
	// 移動入力
	InputMove();

	// 移動量を加味して衝突判定する
	CollisionMapInfo collisionMapInfo;
	collisionMapInfo.move = velocity_;
	// マップ衝突チェック
	CheckMapCollision(collisionMapInfo);

	//判定結果を反映して移動する
	CheckMapMove(collisionMapInfo);

	//天井に接している場合の処理
	CheckMapCeiling(collisionMapInfo);

	// 壁に接している場合の処理

	//接地状態の切り替え

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
	
	// 旋回制御
	AnimateTurn();

	// 着地フラグ
	bool landing = false;

	if (velocity_.y < 0) {
		if (worldTransform_.translation_.y <= 1.0f) {
			landing = true;
		}
	}
	if (onGround_) {
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		}
	} else {
		if (landing) {
			worldTransform_.translation_.y = 1.0f;
			velocity_.x *= (1.0f - kAttenuation);
			velocity_.y = 0.0f;
			onGround_ = true;
		}
	}


}

void Player::Draw() { model_->Draw(worldTransform_, *camera_); }

//移動	
void Player::InputMove() {

	if (onGround_) {
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				acceleration.x += kAcceleration; // 右方向の加速度
				if (velocity_.x > 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft; // 右方向に変更
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
				velocity_ += acceleration; // 速度に加速度を加える
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				acceleration.x -= kAcceleration; // 左方向の加速度
				if (velocity_.x > 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight; // 左方向に変更
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
				velocity_ += acceleration; // 速度に加速度を加える
			}

		} else {
			velocity_.x *= (1.0f - kAttenuation);

			velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
		}

		// ジャンプ
		if (Input::GetInstance()->PushKey(DIK_UP)) {

			velocity_ += Vector3(0, kJmupAcceleration, 0);
		}
	} else {
		velocity_ += Vector3(0, -kGravityAcceleration, 0);
	}
}
// マップ衝突チェック
void Player::CheckMapCollision(CollisionMapInfo& Info) { 
	CheckMapCollisionUp(Info);
	//CheckMapCollisionDown(Info);
	//CheckMapCollisionRight(Info);
	//CheckMapCollisionLeft(Info);
}

// マップ衝突チェック(上)
void Player::CheckMapCollisionUp(CollisionMapInfo& Info) { 

	// 上昇有り
	if (Info.move.y <= 0) {
		return;
	}

	//移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + Info.move, static_cast<Corner>(i));
	}

	
MapChipType mapChipType;
	// 真上のあたり判定を行う
	bool hit = false;
	// 左上点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	//ブロックにhit?
	if (hit) {
		//めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Info.move + Vector3(0, +kHeight / 2.0f,0));
		//めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		Info.move.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));
		//天井にあったていることを記録する
		Info.ceiling = true;

	}



}

//判定結果を反映
void Player::CheckMapMove(const CollisionMapInfo& Info) {

	//移動
	worldTransform_.translation_ += Info.move;

}

// 天井に接している場合の処理
void Player::CheckMapCeiling(const CollisionMapInfo& Info) {

	if (Info.ceiling) {
		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
		velocity_.y = 0.0f;
	}

}


//旋回制御
void Player::AnimateTurn() {
	if (turnTimer_ > 0.0f) {

		turnTimer_ -= 1.0f / 60.0f;

		float destinationRotationYTable[] = {
		    std::numbers::pi_v<float> / 2.0f,       // 右
		    std::numbers::pi_v<float> * 3.0f / 2.0f // 左
		};

		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];

		worldTransform_.rotation_.y = EaselnOut(destinationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);
	}
}

KamataEngine::Vector3 Player::CornerPosition(const KamataEngine::Vector3& center, Corner corner) {

	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0},
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0},
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0},
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0},
	};

	return center + offsetTable[static_cast<uint32_t>(corner)];

}
