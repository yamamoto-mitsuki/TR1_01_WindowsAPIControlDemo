#include "GameScene.h"
#include "Const.h"
#include "MyEngine/Engine.h"
#include "MyEngine/Utils/GlobalVariables.h"
#ifdef USE_IMGUI
#include "externals/imgui/imgui.h"
#endif

//==========================================
// デストラクタ
//==========================================
GameScene::~GameScene() {}

//==========================================
// 終了処理
//==========================================
void GameScene::Finalize() {}

//==========================================
// 初期化
//==========================================
void GameScene::Initialize() {
	// カメラ
	camera_ = std::make_unique<Camera>();
	camera_->Init(0.45f, static_cast<float>(kMainWindowWidth) / static_cast<float>(kMainWindowHeight), 0.1f, 100.0f);
	camera_->SetTranslation({0.0f, 0.0f, -10.0f});
    // デバックカメラ
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Init(0.45f, static_cast<float>(kMainWindowWidth) / static_cast<float>(kMainWindowHeight), 0.1f, 100.0f);
	debugCamera_->SetTranslation({0.0f, 0.0f, -10.0f});
    // 光源
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Init(Engine::GetDxCommon());
	directionalLight_->SetDirection({1.0f, 1.0f, 0.0f});   // 真下から照らす
	directionalLight_->SetColor({1.0f, 1.0f, 1.0f, 1.0f}); // 白色
	directionalLight_->SetIntensity(1.0f);
    // プレイヤー
    player_ = std::make_unique<Player>();
    player_->Initialize();
    // ウィンドウコントロール
    wndController_ = std::make_unique<WindowController>();
    wndController_->Initialize(player_.get());
}

//==========================================
// 更新
//==========================================
void GameScene::Update() {
    // プレイヤー更新
    player_->Update();
    // ウィンドウコントロール更新
    wndController_->Update();
    // カメラ更新
    camera_->Update();
    // 光源更新
    directionalLight_->Update();

    // Enterキーでカメラ切り替え
#ifdef _DEBUG
	if (InputManager::IsKeyTriggered(DIK_RETURN)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}

	if (isDebugCameraActive_) {
		debugCamera_->Update();
	}
#endif

#ifdef USE_IMGUI
    const Vector2& imiPos = { 0.0f,0.0f };
    const Vector2& imiSize = { -1.0f,-1.0f };

    ImGuiManager::AddDrawRequest(ImGuiManager::ImGuiRegion::Right, imiPos,imiSize,[]() {
        GlobalVariables::GetInstance()->Update();
        // FPS表示
        //ImGui::Begin("Debug");
        //ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        //ImGui::Text("DeltaTime: %.4f ms", ImGui::GetIO().DeltaTime * 1000.0f);
        //ImGui::End();
        });
#endif
}

//==========================================
// 描画
//==========================================
void GameScene::Draw() { 
	
}

//==========================================
// 遷移先のシーン
//==========================================
std::unique_ptr<IScene> GameScene::NextScene() { return nullptr; }