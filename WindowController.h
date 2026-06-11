#pragma once
#include <vector>
#include <string>

// 前方宣言
class Player;
class Win32Window;

// ウィンドウ制御クラス
class WindowController {
public:
    // 通知タイプ
    enum class NotificationType {
        None,     // なにもしない
        TryClose, // 閉じようとした
        Found,    // 敵に見つかった
        Caught,   // 敵に捕まった

        Count,    // 参照用のカウント
    };
    // ウィンドウ閉じれるか
    enum class WindowClose {
        NotClose, // 閉じれない
        Normal,   // 閉じれる

        Count,
    };

    // 調整項目
    static inline NotificationType kNotificationType = NotificationType::None;
	static inline WindowClose kWindowCloseState = WindowClose::NotClose;
    static inline int kWindowStartPosX = 0;
    static inline int kWindowStartPosY = 0;

    void Initialize(Player* player);
    void Update();

    // 調整項目を登録・適用・リセット
    static void RegisterGV();
	static void ApplyGV();
	static void ResetGV();

    // ===== ゲッター　=====

    // ===== セッター =====

private:
    // 内部ヘルパー
    void SetCanClose();
    void MoveWindow();
	void UpdateNotification();

    // 包含
    Player* player_ = nullptr;
    Win32Window* window_ = nullptr;

    // ウィンドウ移動後の座標
    int nextWindowPosX_ = 0;
    int nextWindowPosY_ = 0;
    // 通知するかの状態
    NotificationType notificationType = NotificationType::None;
    // ウィンドウを閉じれるか
	WindowClose isClose_ = WindowClose::NotClose;
};