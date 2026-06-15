#include "MyEngine/Engine.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "Const.h"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	// エンジン初期化
	WindowConfig wc;
	wc.title = kMainWindowName;
	wc.width = kMainWindowWidth;
	wc.height = kMainWindowHeight;
#ifdef _DEBUG
    wc.isImGui = true;
#endif
	wc.gameAspectRatio = static_cast<float>(kMainWindowWidth) / static_cast<float>(kMainWindowHeight);
    wc.style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
	Engine::Initialize(wc, std::make_unique<GameScene>());

    /*
    wc.width = 500;
    wc.height = 500;
    wc.title = L"Debug";
#ifdef _DEBUG
    wc.isImGui = true;
    wc.gameViewStart = { 0.0f,0.0f };
    wc.gameViewEnd.x = 0.01f;
    Engine::GetWindowManager()->AddWindow(wc, Engine::GetDxCommon(), std::make_unique<TitleScene>());
#endif
*/

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