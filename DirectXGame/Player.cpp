#include "Player.h"
#include "kamataEngine.h"

using namespace KamataEngine;


void Player::Initialize(KamataEngine::Model* model, uint32_t textureHandle,Camera*camera) { 
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	camera_ = camera;
}

void Player::Update() { worldTransform_.TransferMatrix(); }

void Player::Draw() { 
	model_->Draw(worldTransform_, *camera_, textureHandle_); 
}
