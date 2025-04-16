#include <Windows.h>
#include "kamataEngine.h"
#include "GameScene.h"
using namespace KamataEngine;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	// KamataEngineの初期化
	KamataEngine::Initialize(L" LE2D_19_フクハラ_コウタ");

	

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();	

	// ゲームシーンのインスタンスを生成
	GameScene* gameScene = new GameScene();
	// ゲームシーンの初期化
	gameScene->Initialize();
	//メインループ
	while (true) {
		// エンジンの更新
		if (KamataEngine::Update()) {
			break;
		}

		gameScene->Update();

		// 描画開始
		dxCommon->PreDraw();
		// ここに描画処理を記述

		gameScene->Draw();

		// 描画終了
		dxCommon->PostDraw();
	}
	// ゲームシーンの解放
	delete gameScene;

	gameScene = nullptr;

	//エンジンの終了処理
	KamataEngine::Finalize();
	return 0;
}
