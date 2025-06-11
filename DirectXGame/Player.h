#pragma once
#include "kamataEngine.h"
#include <vector>

class Player {

public:
	void Initialize(KamataEngine::Model* model_, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);
	// 更新
	void Update();
	// 描画
	void Draw();
	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }
	const KamataEngine::Vector3& GetVelocity() const { return velocity_; }
	

private:
	enum class LRDirection { kLeft, kRight };

	// モデル
	KamataEngine::Model* model_ = nullptr;

	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;


	// カメラ
	KamataEngine::Camera* camera_;
	uint32_t textureHandle_ = 0u;

	KamataEngine::Vector3 velocity_ = {}; // 速度

	static inline const float kAcceleration = 0.1f; // 加速度
	static inline const float kAttenuation = 0.1f;  // 減衰率
	static inline const float kTimeTurn = 0.3f;
	static inline const float kGravityAcceleration = 0.1f;//重力
	static inline const float kLimitFallSpeed = 1.0f;//最大速度
	static inline const float kJmupAcceleration = 1.0f;//初速


	LRDirection lrDirection_ = LRDirection::kRight; // 左右の方向

	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0.0f;
	bool onGround_ = true;
};
