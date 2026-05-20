#include "Player.h"
#include "Const.h"
#include "WindowController.h"
#include "NotificationScene.h"
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
    addWindowByNotificationType = kAddWindowType;
    SetCanClose();
    AddWindow();
    MoveWindow();
    kAddWindowType = NotificationType::None;
    addWindowByNotificationType = NotificationType::None;
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

// ===== ウィンドウが増加 =====
void WindowController::AddWindow() {
    // 早期リターン
    if (addWindowByNotificationType == NotificationType::None) {
        return;
    }
    const std::wstring* title = GetUnusedNotificationTitle();
    if (!title) return; // ウィンドウ数の上限に達している

    auto wndManager = Engine::GetWindowManager();
    auto dxCommon = Engine::GetDxCommon();
    // ウィンドウ設定
    WindowConfig wndConfig;
    wndConfig.title = *title;
    wndConfig.width = (int)kSubWindowWidth;
    wndConfig.height = (int)kSubWindowHeight;
    // シーン先設定
    auto scene = std::make_unique<NotificationScene>();
    scene->SetType(addWindowByNotificationType);
    scene->SetWindowName(*title);
    wndManager->AddWindow(wndConfig, dxCommon, std::move(scene));
    // ウィンドウ名を使用中リストに追加
    activeNotificationTitles_.push_back(*title);
    // 調整項目をリセット
    GlobalVariables::ComboItem comboItem;
    comboItem.options = { "None","TryClose","Found","Caught" };
    comboItem.currentIndex = 0; // Noneのインデックス
    GlobalVariables::GetInstance()->SetValue("Window", "Amount", "Add", comboItem);
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

// ===== 使われていない通知ウィンドウのタイトルを取得 =====
const std::wstring* WindowController::GetUnusedNotificationTitle() {
    for (const auto& title : kNotificationWindowNames) {
        auto it = std::find(activeNotificationTitles_.begin(), activeNotificationTitles_.end(), title);
        if (it == activeNotificationTitles_.end()) {
            return &title;
        }
    }
    return nullptr; // 上限に達している
}

//==========================================
// 調整項目の登録・適用
//==========================================

// ===== 登録 =====
void WindowController::RegisterGV() {
    auto gv = GlobalVariables::GetInstance();
    auto gN = "Window";
    auto cN = "Amount";
    gv->AddGroup(gN);
    gv->AddCategory(gN, cN);
    GlobalVariables::ComboItem comboItem;
    comboItem.options = { "None","TryClose","Found","Caught" };
    comboItem.currentIndex = 0;
    gv->AddItem<GlobalVariables::ComboItem>(gN, cN, "Add", comboItem);

    cN = "Position";
    gv->AddItem<int32_t>(gN, cN, "X", kWindowStartPosX);
    gv->AddItem<int32_t>(gN, cN, "Y", kWindowStartPosY);
}

// ===== 適用 =====
void WindowController::ApplyGV() {
    auto gv = GlobalVariables::GetInstance();
    auto gN = "Window";
    auto cN = "Amount";
    int index = gv->GetValue<GlobalVariables::ComboItem>(gN, cN, "Add").currentIndex;
    kAddWindowType = static_cast<NotificationType>(index);
    cN = "Position";
    kWindowStartPosX = gv->GetValue<int32_t>(gN, cN, "X");
    kWindowStartPosY = gv->GetValue<int32_t>(gN, cN, "Y");
}