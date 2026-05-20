#pragma once
#include <string>
#include <array>

// ===== ウィンドウ =====
// ウィンドウ名
inline const std::wstring kMainWindowName = L"TR1";

// --- 通知ウィンドウ ---
// 通知ウィンドウの上限
inline const int kMaxSubWindow = 20;
// 通知ウィンドウのサイズ
inline const float kSubWindowWidth = 300.0f;
inline const float kSubWindowHeight = 150.0f;
// 通知ウィンドウのタイトル
inline const std::array<std::wstring, kMaxSubWindow> kNotificationWindowNames = {
    L"Notification_1", L"Notification_2", L"Notification_3", L"Notification_4", L"Notification_5",
    L"Notification_6", L"Notification_7", L"Notification_8", L"Notification_9", L"Notification_10",
    L"Notification_11",L"Notification_12",L"Notification_13",L"Notification_14",L"Notification_15",
    L"Notification_16",L"Notification_17",L"Notification_18",L"Notification_19",L"Notification_20",
};