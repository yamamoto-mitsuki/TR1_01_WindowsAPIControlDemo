#pragma once
#include <vector>
#include <string>

// 前方宣言
class Player;
class Win32Window;

// ウィンドウ制御クラス
class WindowController {
public:
    // ウィンドウを増やした際に描画するもの
    enum class NotificationType {
        None,     // ウィンドウを増やさない
        TryClose, // 閉じようとした
        Found,    // 敵に見つかった
        Caught,   // 敵に捕まった

        Count,    // 参照用のカウント
    };

    // 調整項目
    static inline NotificationType kAddWindowType = NotificationType::None;

    
    void Initialize(Player* player);
    void Update();

    // 調整項目を登録・適用
    static void RegisterGV();
    static void ApplyGV();

    // ===== ゲッター　=====
    std::vector<std::wstring> GetActiveNotificationTitles() const { return activeNotificationTitles_; }

    // ===== セッター =====
    void SetAddWindowByNotificationType(NotificationType type) { addWindowByNotificationType = type; }

private:
    // 内部ヘルパー
    void SetCanClose();
    void AddWindow();
    const std::wstring* GetUnusedNotificationTitle();

    // 包含
    Player* player_ = nullptr;
    Win32Window* window_ = nullptr;

    // 使用している通知ウィンドウの名前
    std::vector<std::wstring> activeNotificationTitles_;
    // ウィンドウを増やした際に描画するもの
    NotificationType addWindowByNotificationType = NotificationType::None;
};