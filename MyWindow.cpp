#include "MyWindow.h"

//=============================
// ウィンドウプロシージャ
//=============================
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_CREATE: // ウィンドウが作られた瞬間

	case WM_CLOSE: // ×ボタンが押されたとき
    return 0;

	case WM_SYSCOMMAND: // システムメニュー(右クリックなど)操作時
    return 0;

  case WM_ERASEBKGND: // 背景

	case WM_DESTROY: // ウィンドウが破棄されたとき
	  PostQuitMessage(0);
		return 0;
	}

	// 標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

//=============================
// ウィンドウクラス
//=============================
WNDCLASSEX