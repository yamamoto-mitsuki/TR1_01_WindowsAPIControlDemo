#include "Enemy.h"
#include "MyEngine/Engine.h"
#include "Stage.h"
#include "ShapeRenderer.h"
#include <cmath>

namespace {
Vector2 RightOf(const Vector2& dir) { return {-dir.y, dir.x}; }
}

//==========================================
// 初期化
//==========================================
void Enemy::Initialize(const std::vector<Vector2>& route) {
	route_ = route;
	Reset();
}

//==========================================
// スタート状態に戻す
//==========================================
void Enemy::Reset() {
	targetIndex_ = 0;
	state_ = State::Patrol;
	loseTimer_ = 0.0f;
	justFound_ = false;
	// 最初の経由点に配置し、次の経由点を向く
	if (!route_.empty()) {
		position_ = route_[0];
		size_t next = route_.size() > 1 ? 1 : 0;
		facing_ = Normalize(route_[next] - position_);
		targetIndex_ = next;
	}
	// 万一向きがゼロなら右向きにしておく
	if (Length(facing_) < 1e-6f) {
		facing_ = {1.0f, 0.0f};
	}
}

//==========================================
// 視界判定(前方の「矩形」の中にプレイヤーがいるか)
//==========================================
bool Enemy::CanSeePlayer(const Vector2& playerPos, const Stage& stage) const {
	// 敵→プレイヤーのベクトル
	Vector2 toPlayer = playerPos - position_;

	// (1) 前方距離: 向きベクトルへの射影(内積)。
	//     0未満なら後ろ、視界の奥行きより遠ければ範囲外。
	float forward = Dot(toPlayer, facing_);
	if (forward < 0.0f || forward > kVisionLength) {
		return false;
	}

	// (2) 横方向の距離: 向きを90度回した「右方向」への射影。
	//     視界の幅の半分を超えていたら範囲外。
	Vector2 right = RightOf(facing_);
	float side = Dot(toPlayer, right);
	if (std::fabs(side) > kVisionWidth * 0.5f) {
		return false;
	}

	// (3) 間に壁があれば見えない(視線が遮られる)。
	if (stage.IsSightBlocked(position_, playerPos)) {
		return false;
	}

	return true;
}

//==========================================
// 更新(状態遷移 → 向き → 移動)
//==========================================
void Enemy::Update(const Vector2& playerPos, const Stage& stage) {
	justFound_ = false;
	float dt = Engine::GetDeltaTime();

	const bool sees = CanSeePlayer(playerPos, stage);

	// ----- 状態遷移 -----
	if (sees) {
		// 巡回中に見つけた瞬間だけ「発見」フラグを立てる(通知用)
		if (state_ == State::Patrol) {
			justFound_ = true;
		}
		state_ = State::Chase;
		loseTimer_ = 0.0f;
	} else if (state_ == State::Chase) {
		// 見失っても少しの間は追い続け、一定時間で巡回に戻る
		loseTimer_ += dt;
		if (loseTimer_ >= kLoseTime) {
			state_ = State::Patrol;
			RetargetNearest(); // 一番近い経由点から巡回再開
		}
	}

	// ----- 目標方向と速度を決める -----
	Vector2 desiredDir;
	float speed;
	if (state_ == State::Chase) {
		// プレイヤーへ一直線
		desiredDir = Normalize(playerPos - position_);
		speed = kChaseSpeed;
	} else {
		// 経由点へ。到達したら次の経由点へ進む(ループ)
		Vector2 toTarget = route_[targetIndex_] - position_;
		if (Length(toTarget) < kArriveDist) {
			targetIndex_ = (targetIndex_ + 1) % route_.size();
			toTarget = route_[targetIndex_] - position_;
		}
		desiredDir = Normalize(toTarget);
		speed = kPatrolSpeed;
	}

	// ----- 向きを滑らかに変える -----
	// いきなり向きが変わると不自然なので、目標方向へ少しずつ寄せる。
	if (Length(desiredDir) > 1e-6f) {
		facing_ = Normalize(Lerp(facing_, desiredDir, kTurnRate));
	}

	// ----- 移動(壁との衝突は Stage に任せる) -----
	Vector2 delta = facing_ * (speed * dt);
	position_ = stage.MoveAndCollide(position_, delta, kRadius);
}

//==========================================
// 一番近い経由点を次の目標にする
//==========================================
void Enemy::RetargetNearest() {
	float best = 1e30f;
	for (size_t i = 0; i < route_.size(); ++i) {
		float d = Distance(position_, route_[i]);
		if (d < best) {
			best = d;
			targetIndex_ = i;
		}
	}
}

//==========================================
// プレイヤーに触れたか
//==========================================
bool Enemy::IsTouching(const Vector2& playerPos, float playerRadius) const { return Distance(position_, playerPos) < (kRadius + playerRadius); }

//==========================================
// 描画
//==========================================
void Enemy::Draw(const Vector2& viewOrigin, const std::wstring& windowTitle) const {
	// ----- 視界(矩形) -----
	// 視界の中心は「敵から前方へ奥行きの半分だけ進んだ位置」。
	Vector2 visionCenter = position_ + facing_ * (kVisionLength * 0.5f);
	// 向きの角度に合わせて矩形を回転させる。
	float angle = std::atan2(facing_.y, facing_.x);
	uint32_t visionColor = (state_ == State::Chase) ? kAlertColor : kVisionColor;
	ShapeRenderer::DrawRect(visionCenter - viewOrigin, {kVisionLength, kVisionWidth}, visionColor, windowTitle, angle);

	// ----- 体(〇) -----
	ShapeRenderer::DrawCircle(position_ - viewOrigin, kRadius, kBodyColor, windowTitle);
}
