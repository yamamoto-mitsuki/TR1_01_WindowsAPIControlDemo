#include "MyWindow.h"

// ウィンドウの大きさ
int32_t globalWindowWidth = 1280;
int32_t globalWindowHeight = 720;

//=============================
// ウィンドウの作成
//=============================
HWND CreateMyWindow(HINSTANCE hInstance) {
	// ===== ウィンドウクラス =====
	WNDCLASS wc = {};
	// ウィンドウプロシージャ
	wc.lpfnWndProc = WindowProc;
	// ウィンドウクラス名
	wc.lpszClassName = L"AAA";
	// インスタンスハンドル
	wc.hInstance = GetModuleHandle(nullptr);
	// カーソル
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

	// ウィンドウクラスを登録
	RegisterClass(&wc);

	// ===== ウィンドウの生成 =====
	// ウィンドウの大きさ
	RECT wrc = {0, 0, globalWindowWidth, globalWindowHeight};
	// 生成
	HWND hwnd = CreateWindow(
	    wc.lpszClassName,     // 利用するクラス名
	    L"TR1_01",            // タイトルバーの文字
	    WS_OVERLAPPEDWINDOW,  // 普通のウィンドウ
	    CW_USEDEFAULT,        // 表示X座標
	    CW_USEDEFAULT,        // 表示Y座標
	    wrc.right - wrc.left, // ウィンドウ座標
	    wrc.bottom - wrc.top, // ウィンドウ座標
	    nullptr,              // 親ウィンドウハンドル
	    nullptr,              // メニューハンドル
	    wc.hInstance,         // インスタンスハンドル
	    nullptr               // オプション
	);
	// 表示
	ShowWindow(hwnd, SW_SHOW);

	return hwnd;
}

//=============================
// ウィンドウプロシージャ
//=============================
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_CLOSE:      // ×ボタンが押されたとき
		return 0;

	case WM_SYSCOMMAND: // システムメニューからの操作時
		return 0;

	case WM_DESTROY:    // ウィンドウが破棄されたとき
		PostQuitMessage(0);
		return 0;
	}

	// 標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}