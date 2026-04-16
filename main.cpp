#include "MyWindow.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nShowCmd) {
	// ウィンドウ作成
	HWND hwnd = CreateMyWindow(hInstance);

  MSG msg{};
	// ループ
	while (msg.message != WM_QUIT) {
		// Windowにメッセージが来てたら最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
		
    }
	}

	return 0;
}