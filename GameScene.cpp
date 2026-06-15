#include "GameScene.h"
#include "Const.h"
#include "ShapeRenderer.h"
#include "MyEngine/Engine.h"
#include "MyEngine/Utils/GlobalVariables.h"
#ifdef USE_IMGUI
#include "externals/imgui/imgui.h"
#endif

namespace {
constexpr float kCaughtBlackTime = 1.6f;     // 暗転している時間(秒)
constexpr uint32_t kFloorColor = 0x0E1018FF; // 背景(床)の色
constexpr uint32_t kBlackColor = 0x000000FF; // 暗転の色
} 

//==========================================
// デストラクタ
//==========================================
GameScene::~GameScene() {}

//==========================================
// 初期化
//==========================================
void GameScene::Initialize() {
    // カメラ（エディタ用）
	camera_ = std::make_unique<Camera>();
	camera_->Initialize(0.45f, static_cast<float>(kMainWindowWidth) / static_cast<float>(kMainWindowHeight), 0.1f, 100.0f);
	camera_->SetTranslation({0.0f, 0.0f, -10.0f});
	EditorOverlay::SetActiveCamera(camera_.get());
    // ステージ(迷路)
	stage_ = std::make_unique<Stage>();
	stage_->Initialize();
	// プレイヤー
	player_ = std::make_unique<Player>();
	player_->Initialize();
	// 敵(2体)。それぞれ別の通路を上下に巡回する。
	enemies_.clear();
	{
		auto enemy = std::make_unique<Enemy>();
		enemy->Initialize({
		    {580.0f, 220.0f},
            {580.0f, 880.0f}
        }); // A-B の通路
		enemies_.push_back(std::move(enemy));
	}
	{
		auto enemy = std::make_unique<Enemy>();
		enemy->Initialize({
		    {1200.0f, 880.0f},
            {1200.0f, 220.0f}
        }); // C-D の通路
		enemies_.push_back(std::move(enemy));
	}
	// ウィンドウ制御
	wndController_ = std::make_unique<WindowController>();
	wndController_->Initialize(player_.get());

	state_ = State::Playing;
	caughtTimer_ = 0.0f;
}

//==========================================
// 終了処理
//==========================================
void GameScene::Finalize() {}

//==========================================
// 捕まった
//==========================================
void GameScene::OnCaught() {
	state_ = State::Caught;
	caughtTimer_ = kCaughtBlackTime;
	GameNotification::Send("捕まった！", "敵に捕まった。最初からやり直し…");
}

//==========================================
// スタート状態に戻す
//==========================================
void GameScene::ResetStage() {
	player_->Reset();
	for (auto& enemy : enemies_) {
		enemy->Reset();
	}
	state_ = State::Playing;
}

//==========================================
// 更新
//==========================================
void GameScene::Update() {
    // カメラ更新
	camera_->Update();

    switch (state_) {
	case State::Playing: {
        // プレイヤー
		player_->Update(*stage_);
		const Vector2& playerPos = player_->GetPosition();
        // 敵
		for (auto& enemy : enemies_) {
			enemy->Update(playerPos,*stage_);
            // 見つかった瞬間通知
			if (enemy->JustFound()) {
				GameNotification::Send("見つかった！", "敵に発見された。逃げろ！");
            }
            // 捕まった
			if (enemy->IsTouching(playerPos, player_->GetRadius())) {
				OnCaught();
            }
        }
        // ウィンドウ追従
		wndController_->Update();
        // ゴール判定
		if (state_ == State::Playing && stage_->ReachedGoal(playerPos, player_->GetRadius())) {
			state_ = State::Clear;
			wndController_->SetCanClose(true); // 閉じられるようにする
			GameNotification::Send("クリア！", "脱出成功！ウィンドウを閉じられるようになった。");
		}
		break;
    }

    case State::Caught: {
		// 暗転中。カウントが終わったら最初から。
		caughtTimer_ -= Engine::GetDeltaTime();
		if (caughtTimer_ <= 0.0f) {
			ResetStage();
		}
		// ビューはプレイヤー位置に合わせておく
		wndController_->Update();
		break;
    }

    case State::Clear: {
		wndController_->Update();
		break;
	}

    }
}

//==========================================
// 描画
//==========================================
void GameScene::Draw() { 
	const Vector2 viewOrigin = wndController_->GetViewOrigin();
	const Vector2 viewSize = wndController_->GetViewSize();
    // 早期リターン
	if (viewSize.x <= 0.0f || viewSize.y <= 0.0f) {
		return;
	}
	const Vector2 viewCenter = viewSize * 0.5f;

	// 背景(床)
	ShapeRenderer::DrawRect(viewCenter, viewSize, kFloorColor, windowTitle_);

	// ステージ(ゴール・壁)
	stage_->Draw(viewOrigin, windowTitle_);

	// 敵(視界 → 体)
	for (auto& enemy : enemies_) {
		enemy->Draw(viewOrigin, windowTitle_);
	}

	// プレイヤー
	player_->Draw(viewOrigin, windowTitle_);

	// 捕まったら画面全体を真っ暗にする
	if (state_ == State::Caught) {
		ShapeRenderer::DrawRect(viewCenter, viewSize, kBlackColor, windowTitle_);
	}
}

//==========================================
// 遷移先のシーン
//==========================================
std::unique_ptr<IScene> GameScene::NextScene() { return nullptr; }