#include "TitleScene.h"
#include "MyEngine/Engine.h"

//==========================================
// デストラクタ
//==========================================
TitleScene::~TitleScene() {}

//==========================================
// 終了処理
//==========================================
void TitleScene::Finalize() {}

//==========================================
// 初期化
//==========================================
void TitleScene::Initialize() {
	// カメラ
	camera_ = std::make_unique<Camera>();
	camera_->Init(0.45f, 1280.0f / 720.0f, 0.1f, 100.0f);
	camera_->SetTranslation({0.0f, 0.0f, -10.0f});

	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Init(0.45f, 1280.0f / 720.0f, 0.1f, 100.0f);
	debugCamera_->SetTranslation({0.0f, 0.0f, -10.0f});

	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Init(Engine::GetDxCommon());
	directionalLight_->SetDirection({1.0f, 1.0f, 0.0f});   // 真下から照らす
	directionalLight_->SetColor({1.0f, 1.0f, 1.0f, 1.0f}); // 白色
	directionalLight_->SetIntensity(1.0f);

	// ImGui
	auto* gv = GlobalVariables::GetInstance();
	// グループとカテゴリを登録
	gv->AddGroup("GameScene");
	gv->AddCategory("GameScene", "Test");
	gv->AddCategory("GameScene", "Test2");
	// 初期値をセット
	gv->SetValue("GameScene", "DirectionalLight", "Direction", Vector3{1.0f, 1.0f, 0.0f});
	gv->SetValue("GameScene", "DirectionalLight", "Intensity", 1.0f);
	gv->SetValue("GameScene", "Camera", "Translation", Vector3{0.0f, 0.0f, -10.0f});
}

//==========================================
// 更新
//==========================================
void TitleScene::Update() {
#ifdef _DEBUG
	// デバックカメラ切り替え
	if (InputManager::IsKeyTriggered(DIK_RETURN)) {
		LogManager::Log("TitleScene: ENTERキーが押されました");
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
	
	if (isDebugCameraActive_) {
		debugCamera_->Update();
	}
#endif
	// カメラの更新
	camera_->Update();
	// 光源の更新
	directionalLight_->Update();


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
void TitleScene::Draw() { 
#ifdef _DEBUG
	// デバッググリッド表示
	// アクティブなカメラを使う
	Camera* activeCamera = isDebugCameraActive_ ? debugCamera_.get() : camera_.get();

	DebugRender::LineListConfig grid;
	grid.camera = activeCamera;
	grid.windowTitle = windowTitle_;
	grid.fadeStartDistance = 20.0f;
	grid.fadeEndDistance = 60.0f;

	// グリッドの設定
	constexpr int kHalfCount = 50;   // 原点から何マス分描くか
	constexpr float kSpacing = 1.0f; // 1マスの大きさ
	constexpr float kExtent = kHalfCount * kSpacing;
	constexpr uint32_t kGridColor = 0x666666FF;    // 通常グリッド色(暗いグレー)
	constexpr uint32_t kAxisXColor = 0xFF0000FF;   // X軸色(赤)
	constexpr uint32_t kAxisZColor = 0x0000FFFF;   // Z軸色(青)

	for (int i = -kHalfCount; i <= kHalfCount; ++i) {
		float f = static_cast<float>(i) * kSpacing;

		// Z軸に平行な線(X方向に並ぶ)
		uint32_t colorX = (i == 0) ? kAxisZColor : kGridColor; // i=0はZ軸そのもの
		grid.lines.push_back({{f, 0.0f, -kExtent}, {f, 0.0f, kExtent}, colorX});

		// X軸に平行な線(Z方向に並ぶ)
		uint32_t colorZ = (i == 0) ? kAxisXColor : kGridColor; // i=0はX軸そのもの
		grid.lines.push_back({{-kExtent, 0.0f, f}, {kExtent, 0.0f, f}, colorZ});
	}

	DebugRender::DrawLines(grid);
#endif
}

//==========================================
// 遷移先のシーン
//==========================================
std::unique_ptr<IScene> TitleScene::NextScene() { return nullptr; }