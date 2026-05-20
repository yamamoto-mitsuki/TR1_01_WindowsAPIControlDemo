#include "NotificationScene.h"
#include "Const.h"
#include "MyEngine/Render/DebugRender.h"

//==========================================
// デストラクタ
//==========================================
NotificationScene::~NotificationScene() = default;

//==========================================
// 終了処理
//==========================================
void NotificationScene::Finalize() {}

//==========================================
// 初期化
//==========================================
void NotificationScene::Initialize() {

}

//==========================================
// 更新
//==========================================
void NotificationScene::Update() {

}

//==========================================
// 描画
//==========================================
void NotificationScene::Draw() {
    // 早期リターン
    if (type_ == NotificationType::None) {
        return;
    }

    DebugRender::Rect2dConfig rectConfig;
    rectConfig.position = { kSubWindowWidth / 2.0f, kSubWindowHeight / 2.0f };
    rectConfig.size = { kSubWindowWidth, kSubWindowHeight };
    rectConfig.windowTitle = windowName_;

    switch (type_) {
    case NotificationType::None:

        break;
    // 閉じようとしたとき
    case NotificationType::TryClose:
        rectConfig.color = 0xFF0000FF; // 赤色
        DebugRender::DrawRect2d(rectConfig);
        break;

    case NotificationType::Found:
        rectConfig.color = 0x000000FF; // 黒色
        DebugRender::DrawRect2d(rectConfig);
        break;

    case NotificationType::Caught:
        rectConfig.color = 0xFFFFFFFF; // 白色
        DebugRender::DrawRect2d(rectConfig);
        break;
    }
}

//==========================================
// シーン遷移
//==========================================
std::unique_ptr<IScene> NotificationScene::NextScene() { return nullptr; }