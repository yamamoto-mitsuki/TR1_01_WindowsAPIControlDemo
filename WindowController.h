#pragma once

// 前方宣言
class Player;
class Win32Window;

// ウィンドウ制御クラス
class WindowController {
public:
    void Initialize(Player* player);
    void Update();

    // 調整項目を登録・適用
    static void RegisterGV();
    static void ApplyGV();

private:
    // 内部ヘルパー
    void SetCanClose();

    // 包含
    Player* player_ = nullptr;
    Win32Window* window_ = nullptr;

    bool isWindowAdd = false;
};