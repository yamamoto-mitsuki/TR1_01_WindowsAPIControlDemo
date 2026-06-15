#include <Windows.h>
#include "WindowController.h"
#include "MyEngine/Engine.h"
#include "Const.h"
#include "Player.h"
#include <cmath>

//==========================================
// 初期化
//==========================================
void WindowController::Initialize(Player* player) {
	player_ = player;
	window_ = Engine::GetWindowManager()->GetWindowByTitle(kMainWindowName);

	// タイトルバーをドラッグして手で動かす移動はロックする。
	// (ウィンドウの移動はプレイヤー追従でのみ行い、挙動を分かりやすくする)
	if (window_) {
		window_->SetPositionLock(true);
	}

	SetupCloseLock();
}

//==========================================
// 更新
//==========================================
void WindowController::Update() {
	if (!window_) {
		window_ = Engine::GetWindowManager()->GetWindowByTitle(kMainWindowName);
	}
	SetupCloseLock();
	UpdateViewport();
}

//==========================================
// 閉じる制御を仕込む(1度だけ)
//   onCanClose_ : false を返すと×ボタンを押しても閉じない
//   onTryClose_ : 閉じられなかったときに呼ばれる(通知を出す)
//==========================================
void WindowController::SetupCloseLock() {
	if (closeHooked_ || !window_) {
		return;
	}
	window_->onCanClose_ = [this]() { return canClose_; };
	window_->onTryClose_ = []() { GameNotification::Send("まだ閉じられない！", "ゴールして脱出するまでウィンドウは閉じられない…"); };
	closeHooked_ = true;
}

//==========================================
// ビューポート更新(プレイヤー追従でウィンドウを移動)
//==========================================
void WindowController::UpdateViewport() {
	if (!window_ || !player_) {
		return;
	}

    viewSize_ = {Engine::GetGameViewWidth(), Engine::GetGameViewHeight()};
	if (viewSize_.x <= 0.0f || viewSize_.y <= 0.0f) {
		return;
	}
	// プレイヤーがビューの中心に来るように、ビュー左上のワールド座標を決める。
	Vector2 origin = player_->GetPosition() - viewSize_ * 0.5f;

    // ワールドの外を映さないようにクランプする。
	// 端に来るとウィンドウはそれ以上動かず、プレイヤーがビュー端へ寄っていく。
	origin.x = std::clamp(origin.x, 0.0f, kWorldWidth - viewSize_.x);
	origin.y = std::clamp(origin.y, 0.0f, kWorldHeight - viewSize_.y);
	viewOrigin_ = origin;
	HWND hwnd = window_->GetHWND();
	if (!hwnd) {
		return;
	}
	RECT windowRect;
	GetWindowRect(hwnd, &windowRect);
	POINT clientTopLeft = {0, 0};
	ClientToScreen(hwnd, &clientTopLeft);
	int borderX = clientTopLeft.x - windowRect.left;
	int borderY = clientTopLeft.y - windowRect.top;

    // ワールド座標 = デスクトップ座標 として扱うので、
	// クライアント左上を viewOrigin_ のデスクトップ位置に置く。
	int outerX = static_cast<int>(viewOrigin_.x) - borderX;
	int outerY = static_cast<int>(viewOrigin_.y) - borderY;

    SetWindowPos(hwnd, nullptr, outerX, outerY, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}