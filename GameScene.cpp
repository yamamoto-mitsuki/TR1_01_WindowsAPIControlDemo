#include "GameScene.h"
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
	camera_->Init(0.45f, 1280.0f / 720.0f, 0.1f, 100.0f);
	camera_->SetTranslation({0.0f, 0.0f, -10.0f});
    // デバックカメラ
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Init(0.45f, 1280.0f / 720.0f, 0.1f, 100.0f);
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

    // --- 調整項目 ---
	auto* gv = GlobalVariables::GetInstance();
	// グループとカテゴリを登録
	gv->AddGroup("GameScene");
	gv->AddCategory("GameScene", "Test");
	gv->AddCategory("GameScene", "Test2");
	// 初期値をセット
	gv->SetValue("GameScene", "DirectionalLight", "Direction", Vector3{1.0f, 1.0f, 0.0f});
	gv->SetValue("GameScene", "DirectionalLight", "Intensity", 1.0f);
	gv->SetValue("GameScene", "Camera", "Translation", Vector3{0.0f, 0.0f, -10.0f});
    GlobalVariables::ComboItem goalCombo;
    goalCombo.options = { "Not", "Goal" };
    goalCombo.currentIndex = 0;
    gv->AddItem("GameScene", "Player", "GoalState", goalCombo);
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

    // ImGuiの描画リクエスト送信
#ifdef USE_IMGUI
    const Vector2& imiPos = { 0.0f,0.0f };
    const Vector2& imiSize = { -1.0f,-1.0f };
   
    ImGuiManager::AddDrawRequest(ImGuiManager::ImGuiRegion::Right, imiPos,imiSize,[]() {
        GlobalVariables::GetInstance()->Update();
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