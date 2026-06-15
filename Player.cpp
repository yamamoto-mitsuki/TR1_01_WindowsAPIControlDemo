#include "Player.h"
#include "MyEngine/Engine.h"
#include "ShapeRenderer.h"

//==========================================
// 初期化
//==========================================
void Player::Initialize() { position_ = kInitPosition; }

//==========================================
// スタート地点に戻す
//==========================================
void Player::Reset() { position_ = kInitPosition; }

//==========================================
// 更新(入力→移動→壁との衝突解決)
//==========================================
void Player::Update(const Stage& stage) {
	// ----- 入力から移動方向を作る(WASD / 矢印キー) -----
	Vector2 dir = {0.0f, 0.0f};
	if (InputManager::IsKeyPressed(DIK_A) || InputManager::IsKeyPressed(DIK_LEFT)) {
		dir.x -= 1.0f;
	}
	if (InputManager::IsKeyPressed(DIK_D) || InputManager::IsKeyPressed(DIK_RIGHT)) {
		dir.x += 1.0f;
	}
	if (InputManager::IsKeyPressed(DIK_W) || InputManager::IsKeyPressed(DIK_UP)) {
		dir.y -= 1.0f; // スクリーン座標はY下向きが正なので、上は -Y
	}
	if (InputManager::IsKeyPressed(DIK_S) || InputManager::IsKeyPressed(DIK_DOWN)) {
		dir.y += 1.0f;
	}

	// 斜め移動でも速くならないように正規化
	dir = Normalize(dir);

	// 速度 × 経過時間 で移動量を出す(フレームレートに依存しない)
	float dt = Engine::GetDeltaTime();
	Vector2 delta = dir * (kSpeed * dt);

	// 壁との衝突は Stage に任せる
	position_ = stage.MoveAndCollide(position_, delta, kRadius);
}

//==========================================
// 描画(ワールド座標 → ビュー内のスクリーン座標)
//==========================================
void Player::Draw(const Vector2& viewOrigin, const std::wstring& windowTitle) const { ShapeRenderer::DrawCircle(position_ - viewOrigin, kRadius, kColor, windowTitle); }