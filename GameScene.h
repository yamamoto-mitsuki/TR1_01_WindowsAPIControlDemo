#pragma once
#include "Player.h"
#include "WindowController.h"
#include "MyEngine/Camera/Camera.h"
#include "MyEngine/Camera/DebugCamera.h"
#include "MyEngine/Render/ModelManager.h"
#include "MyEngine/Render/DirectionalLight.h"
#include "MyEngine/Scene/IScene.h"
#include <cstdint>
#include <memory>

class GameScene : public IScene {
public:
	~GameScene() override;
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
	std::unique_ptr<IScene> NextScene() override;

private:
	std::unique_ptr<Camera> camera_;
	std::unique_ptr<DebugCamera> debugCamera_;
    std::unique_ptr<Player> player_;
    std::unique_ptr<WindowController> wndController_;

	bool isDebugCameraActive_ = false;
};