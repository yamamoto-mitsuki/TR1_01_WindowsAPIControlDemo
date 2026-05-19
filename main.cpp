#include "MyEngine/Engine.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "Const.h"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	// エンジン初期化
	WindowConfig wc;
	wc.title = kMainWindowName;
	wc.width = 1280;
	wc.height = 720;
	wc.gameAspectRatio = 1280.0f / 720.0f;
#ifdef _DEBUG
	wc.isImGui = true;
	wc.gameViewStart = { 0.0f,0.0f };
	wc.gameViewEnd.x = 0.75f;
#endif
	Engine::Initialize(wc, std::make_unique<GameScene>());

	while (Engine::ProcessMessage()) {
		Engine::BeginFrame();

		// 更新処理
		Engine::GetWindowManager()->UpdateAll();
		// 描画処理
		Engine::GetWindowManager()->DrawAll();

		Engine::EndFrame();
	}

	Engine::Finalize();
	return 0;
}