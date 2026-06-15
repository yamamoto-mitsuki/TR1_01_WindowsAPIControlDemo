#include "Stage.h"
#include "Const.h"
#include "ShapeRenderer.h"
#include <algorithm>

namespace {
// ===== 色 =====
constexpr uint32_t kWallColor = 0x3A4A6BFF; // 壁
constexpr uint32_t kGoalColor = 0x33DD66FF; // ゴール
// ===== 外周の厚み =====
constexpr float kBorder = 40.0f;
}

//=============================================================================
// 初期化
//=============================================================================
void Stage::Initialize() {
	walls_.clear();

	// ===== 外周の壁 =====
    // 上
	walls_.push_back({
	    {0.0f,        0.0f   },
        {kWorldWidth, kBorder}
    });
    // 下
	walls_.push_back({
	    {0.0f,        kWorldHeight - kBorder},
        {kWorldWidth, kBorder               }
    });
	// 左
	walls_.push_back({
	    {0.0f,    0.0f        },
        {kBorder, kWorldHeight}
    });
	// 右
	walls_.push_back({
	    {kWorldWidth - kBorder, 0.0f        },
        {kBorder,               kWorldHeight}
    });

    // ===== 内部の迷路(蛇行する「くし(コーム)」型) =====
	// 上から伸びる壁と下から伸びる壁を交互に置き、必ず左→右に抜けられる
	// S字の通路を作る。これで「解けない迷路」にならない
	const float kToothH = 720.0f; // 壁の縦の長さ
	walls_.push_back({
	    {400.0f,  kBorder},
        {kBorder, kToothH}
    }); // A:上から(下が開く)
	walls_.push_back({
	    {720.0f,  kWorldHeight - kBorder - kToothH},
        {kBorder, kToothH                         }
    }); // B:下から(上が開く)
	walls_.push_back({
	    {1040.0f, kBorder},
        {kBorder, kToothH}
    }); // C:上から(下が開く)
	walls_.push_back({
	    {1360.0f, kWorldHeight - kBorder - kToothH},
        {kBorder, kToothH                         }
    }); // D:下から(上が開く)

	// ===== ゴール(右奥) =====
	goal_ = {
	    {1700.0f, 480.0f},
        {100.0f,  100.0f}
    };
}

//==========================================
// 円とAABBの重なり判定(中心c・半径r)
//==========================================
bool Stage::CircleHitsBox(const Vector2& c, float r, const Box& box) {
	// 矩形内に最も近い点へクランプして距離を測る
	float nearestX = std::clamp(c.x, box.pos.x, box.pos.x + box.size.x);
	float nearestY = std::clamp(c.y, box.pos.y, box.pos.y + box.size.y);
	float dx = c.x - nearestX;
	float dy = c.y - nearestY;
	return (dx * dx + dy * dy) < (r * r);
}

//==========================================
// 移動 + 壁との衝突解決(X軸→Y軸の順)
//==========================================
Vector2 Stage::MoveAndCollide(const Vector2& pos, const Vector2& delta, float radius) const {
	Vector2 result = pos;
	const float hr = radius; // 当たり判定は半径を半サイズにした正方形で近似する

	// AABB同士の重なり判定(エンティティの中心cx,cy・半サイズhr)
	auto overlap = [hr](float cx, float cy, const Box& b) { return (cx + hr > b.pos.x) && (cx - hr < b.pos.x + b.size.x) && (cy + hr > b.pos.y) && (cy - hr < b.pos.y + b.size.y); };

	// ----- X軸 -----
	result.x += delta.x;
	for (const Box& b : walls_) {
		if (overlap(result.x, pos.y, b)) {
			if (delta.x > 0.0f) {
				result.x = b.pos.x - hr; // 右へ進んで左面に当たった
			} else if (delta.x < 0.0f) {
				result.x = b.pos.x + b.size.x + hr; // 左へ進んで右面に当たった
			}
		}
	}

	// ----- Y軸 -----
	result.y += delta.y;
	for (const Box& b : walls_) {
		if (overlap(result.x, result.y, b)) {
			if (delta.y > 0.0f) {
				result.y = b.pos.y - hr; // 下へ進んで上面に当たった
			} else if (delta.y < 0.0f) {
				result.y = b.pos.y + b.size.y + hr; // 上へ進んで下面に当たった
			}
		}
	}

	// ----- ワールド外に出ないようにクランプ -----
	result.x = std::clamp(result.x, hr, kWorldWidth - hr);
	result.y = std::clamp(result.y, hr, kWorldHeight - hr);
	return result;
}

//==========================================
// 点が壁の内側か
//==========================================
bool Stage::IsInsideWall(const Vector2& point) const {
	for (const Box& b : walls_) {
		if (point.x >= b.pos.x && point.x <= b.pos.x + b.size.x && point.y >= b.pos.y && point.y <= b.pos.y + b.size.y) {
			return true;
		}
	}
	return false;
}

//==========================================
// 視線(線分)が壁で遮られているか
//   線分を細かくサンプリングして、壁の中に入る点があれば遮断とみなす。
//==========================================
bool Stage::IsSightBlocked(const Vector2& a, const Vector2& b) const {
	float dist = Distance(a, b);
	int steps = std::max(2, static_cast<int>(dist / 16.0f));
	for (int i = 1; i < steps; ++i) {
		float t = static_cast<float>(i) / static_cast<float>(steps);
		if (IsInsideWall(Lerp(a, b, t))) {
			return true;
		}
	}
	return false;
}

//==========================================
// ゴール到達判定
//==========================================
bool Stage::ReachedGoal(const Vector2& pos, float radius) const { return CircleHitsBox(pos, radius, goal_); }

//==========================================
// 描画
//==========================================
void Stage::Draw(const Vector2& viewOrigin, const std::wstring& windowTitle) const {
	// ゴール(最初は画面外にあるので「見えない」。ウィンドウを動かして近づくと現れる)
	Vector2 goalCenter = {goal_.pos.x + goal_.size.x * 0.5f, goal_.pos.y + goal_.size.y * 0.5f};
	ShapeRenderer::DrawRect(goalCenter - viewOrigin, goal_.size, kGoalColor, windowTitle);

	// 壁
	for (const Box& b : walls_) {
		Vector2 center = {b.pos.x + b.size.x * 0.5f, b.pos.y + b.size.y * 0.5f};
		ShapeRenderer::DrawRect(center - viewOrigin, b.size, kWallColor, windowTitle);
	}
}