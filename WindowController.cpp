#include "Player.h"
#include "Const.h"
#include "WindowController.h"
#include "MyEngine/Engine.h"


//==========================================
// 初期化
//==========================================
void  WindowController::Initialize(Player* player) {
    player_ = player;
}

//==========================================
// 更新
//==========================================
void WindowController::Update() {
    SetCanClose();
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
            return player_->GetGoalState() == Player::GoalState::Goal;
        };
    }
}