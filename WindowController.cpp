#include "WindowController.h"
#include "Player.h"
#include "MyEngine/Engine.h"

//==========================================
// 初期化
//==========================================
void  WindowController::Initialize() {

}

//==========================================
// 更新
//==========================================
void WindowController::Update() {

}

//==========================================
// 内部ヘルパー
//==========================================

// ===== ウィンドウの閉じれない制限 =====
void WindowController::SetCanClose() {
    Player::GoalState goalState = player_->GetGoalState();
    Win32Window mainWindow = Engine::GetWindowManager()
        ->GetWindowByTitle(L"Title");
}