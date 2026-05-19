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
	camera_->Init(0.45f, 300.0f / 720.0f, 0.1f, 100.0f);
	camera_->SetTranslation({0.0f, 0.0f, -10.0f});

	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Init(0.45f, 1280.0f / 720.0f, 0.1f, 100.0f);
	debugCamera_->SetTranslation({0.0f, 0.0f, -10.0f});

	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Init(Engine::GetDxCommon());
	directionalLight_->SetDirection({1.0f, 1.0f, 0.0f});  // 真下から照らす
	directionalLight_->SetColor({1.0f, 1.0f, 1.0f, 1.0f}); // 白色
	directionalLight_->SetIntensity(1.0f);

	// モデル読み込み
	modelHandle_ = ModelManager::Load("resources/plane.obj");

	// 描画設定
	modelConfig_.handle = modelHandle_;
	modelConfig_.windowTitle = windowTitle_;
	modelConfig_.shadingModel = ShadingModel::HalfLambert;
	modelConfig_.camera = camera_.get();
	modelConfig_.transform.translation = {0.0f, 0.0f, 0.0f};
	modelConfig_.transform.scale = {1.0f, 1.0f, 1.0f};
	modelConfig_.transform.rotation = {0.0f, 0.0f, 0.0f};
	modelConfig_.directionalLight = directionalLight_.get();

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
void GameScene::Update() {
#ifdef _DEBUG
	// Enterキーでカメラ切り替え
	if (InputManager::IsKeyTriggered(DIK_RETURN)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
		modelConfig_.camera = isDebugCameraActive_ ? debugCamera_.get() : camera_.get();
	}
	if (isDebugCameraActive_) {
		debugCamera_->Update();
	}
#endif



	camera_->Update();
	directionalLight_->Update();

	
}

//==========================================
// 描画
//==========================================
void GameScene::Draw() { 
	ModelManager::DrawModel(modelConfig_); 
}

//==========================================
// 遷移先のシーン
//==========================================
std::unique_ptr<IScene> GameScene::NextScene() { return nullptr; }