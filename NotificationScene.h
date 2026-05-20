#pragma once
#include "WindowController.h"
#include "MyEngine/Camera/Camera.h"
#include "MyEngine/Camera/DebugCamera.h"
#include "MyEngine/Render/ModelManager.h"
#include "MyEngine/Scene/IScene.h"
#include <cstdint>
#include <memory>


class NotificationScene : public IScene {
public:
    using NotificationType = WindowController::NotificationType;

    ~NotificationScene() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Finalize() override;
    std::unique_ptr<IScene> NextScene() override;

    // ===== セッター =====
    void SetType(NotificationType type) { type_ = type; }
    void SetWindowName(const std::wstring& name) { windowName_ = name; }

private:
   NotificationType type_ = NotificationType::None;
   std::wstring windowName_ = L"";
};