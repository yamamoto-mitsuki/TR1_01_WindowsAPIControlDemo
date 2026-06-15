#pragma once
#include "Player.h"
#include "WindowController.h"
#include "Enemy.h"
#include "MyEngine/Camera/Camera.h"
#include "MyEngine/Camera/DebugCamera.h"
#include "MyEngine/Render/ModelManager.h"
#include "MyEngine/Render/DirectionalLight.h"
#include "MyEngine/Scene/IScene.h"
#include <cstdint>
#include <memory>

class GameScene : public IScene {
public:
	~GameScene() override;
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	std::unique_ptr<IScene> NextScene() override;

private:
	// ゲームの状態
	enum class State {
		Playing, // プレイ中
		Caught,  // 捕まった(暗転中)
		Clear,   // クリア
	};

     // 捕まったときの処理(暗転を始める)
	void OnCaught();
	// スタート状態に戻す
	void ResetStage();

	std::unique_ptr<Camera> camera_; // エディタ用(2D描画では未使用)
	std::unique_ptr<Stage> stage_;
	std::unique_ptr<Player> player_;
	std::vector<std::unique_ptr<Enemy>> enemies_;
	std::unique_ptr<WindowController> wndController_;

	State state_ = State::Playing;
	float caughtTimer_ = 0.0f; // 暗転の残り時間
};