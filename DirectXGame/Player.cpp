#define NOMINMAX
#include "Player.h"

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
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
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
		if (Input::GetInstance()->PushKey(DIK_UP)) {
		
			velocity_ += Vector3(0, kJmupAcceleration, 0);

		}
	} else {
		velocity_ += Vector3(0 ,- kGravityAcceleration, 0);
	}

	worldTransform_.translation_ += velocity_;
	if (turnTimer_ > 0.0f) {

		turnTimer_ -= 1.0f / 60.0f;

		float destinationRotationYTable[] = {
		    std::numbers::pi_v<float> / 2.0f,       // 右
		    std::numbers::pi_v<float> * 3.0f / 2.0f // 左
		};

		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];

		worldTransform_.rotation_.y = EaselnOut(destinationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);
	}
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
