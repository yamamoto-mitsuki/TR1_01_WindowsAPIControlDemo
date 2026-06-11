#include "Player.h"
#include "Const.h"
#include "WindowController.h"
#include "MyEngine/Engine.h"
#include <Windows.h>
#include <cstdint>

//==========================================
// 初期化
//==========================================
void  WindowController::Initialize(Player* player) {
    player_ = player;
    RegisterGV();
}

//==========================================
// 更新
//==========================================
void WindowController::Update() {
    ApplyGV();
    notificationType = kNotificationType;
	isClose_ = kWindowCloseState;
    SetCanClose();
    MoveWindow();
	UpdateNotification();
	ResetGV();
}

//==========================================
// 内部ヘルパー
//==========================================
// ===== ウィンドウの閉じれない制限 =====
void WindowController::SetCanClose() {
    Win32Window* mainWindow = Engine::GetWindowManager()->GetWindowByTitle(kMainWindowName);

    // クリアするまでウィンドウが閉じれない
    if (mainWindow) {
        mainWindow->onCanClose_ = [this]() { 
            bool isClose = true;
			if (isClose_ == WindowClose::NotClose) {
				isClose = false;
            }

            return isClose;
        };
    }
}

// ===== ウィンドウを移動 =====
void WindowController::MoveWindow() {
    auto wndManager = Engine::GetWindowManager();
    auto mainWnd = wndManager->GetWindowByTitle(kMainWindowName);
    // 早期リターン
    if (!mainWnd) return;
    if (kWindowStartPosX == 0 && kWindowStartPosY == 0) return;

    // 対象のウィンドウの座標を取得
    RECT rect;
    GetWindowRect(mainWnd->GetHWND(), &rect);
    SetWindowPos(mainWnd->GetHWND(), nullptr, rect.left + kWindowStartPosX, rect.top + kWindowStartPosY, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
}

// ===== 通知を出す =====
void WindowController::UpdateNotification() { 
    switch (notificationType) { 
    
    case NotificationType::TryClose:
		GameNotification::Send("閉じようとした");
		break;

    case NotificationType::Found:
		GameNotification::Send("見つかった");
		break;

    case NotificationType::Caught:
		GameNotification::Send("捕まった");
		break;

    default:
        break;
    }

}

//==========================================
// 調整項目の登録・適用・リセット
//==========================================

// ===== 登録 =====
void WindowController::RegisterGV() {
    auto gv = GlobalVariables::GetInstance();
	auto sN = "GameScene";
    auto gN = "Window";
    GlobalVariables::ComboItem comboItem;
	// 通知状態
    comboItem.options = { "None","TryClose","Found","Caught" };
    comboItem.currentIndex = 0;
	gv->Scene(sN).Group(gN).Add<GlobalVariables::ComboItem>("State/Notification", comboItem);
    // 閉じれるか
	comboItem.options = {"NotClose", "CanClose"};
	comboItem.currentIndex = 0;
	gv->Scene(sN).Group(gN).Add<GlobalVariables::ComboItem>("State/Close", comboItem);
    // 位置
	gv->Scene(sN).Group(gN).Add<int32_t>("PosX", kWindowStartPosX);
	gv->Scene(sN).Group(gN).Add<int32_t>("PosY", kWindowStartPosX);
}

// ===== 適用 =====
void WindowController::ApplyGV() {
    auto gv = GlobalVariables::GetInstance();
	auto sN = "GameScene";
    auto gN = "Window";
    // 通知状態
	int index = gv->Get<GlobalVariables::ComboItem>(sN, gN, "State/Notification").currentIndex;
    kNotificationType = static_cast<NotificationType>(index);
    // 閉じれるか
	index = gv->Get<GlobalVariables::ComboItem>(sN, gN, "State/Close").currentIndex;
	kWindowCloseState = static_cast<WindowClose>(index);
    // 座標
	kWindowStartPosX = gv->Get<int32_t>(sN, gN, "PosX");
	kWindowStartPosY = gv->Get<int32_t>(sN, gN, "PosY");
}

// ===== リセット =====
void WindowController::ResetGV() {
	auto gv = GlobalVariables::GetInstance();
	auto sN = "GameScene";
	auto gN = "Window";
    // 通知状態
	GlobalVariables::ComboItem comboItem;
	comboItem.options = {"None", "TryClose", "Found", "Caught"};
	comboItem.currentIndex = 0;
	gv->Set<GlobalVariables::ComboItem>(sN, gN, "State/Notification", comboItem);
}