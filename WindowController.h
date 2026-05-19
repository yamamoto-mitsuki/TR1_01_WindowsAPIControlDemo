#pragma once

// 前方宣言
class Player;
class Win32Window;

// ウィンドウ制御クラス
class WindowController {
public:
    void Initialize(Player* player);
    void Update();

private:
    // 内部ヘルパー
    void SetCanClose();

    // 包含
    Player* player_ = nullptr;
    Win32Window* window_ = nullptr;
};