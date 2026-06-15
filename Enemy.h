#pragma once
#include "MyEngine/Math/Vector2.h"
#include <cstdint>
#include <string>
#include <vector>

// 前方宣言
class Stage;

//==========================================
// 敵(〇・プレイヤーとは別の色)
//   ・Patrol : 経由点(ルート)を巡回する
//   ・Chase  : プレイヤーを見つけたら追いかける
//   前方に「矩形の視界」を持ち、その中にプレイヤーが入ると発見する。
//==========================================
class Enemy {
public:
	// 行動状態
	enum class State {
		Patrol, // 巡回
		Chase,  // 追跡
	};

	// ===== 調整項目 =====
	static inline float kPatrolSpeed = 130.0f;  // 巡回速度(px/秒)
	static inline float kChaseSpeed = 210.0f;   // 追跡速度(px/秒)。プレイヤーより少し遅い
	static inline float kRadius = 18.0f;        // 当たり判定・見た目の半径
	static inline float kVisionLength = 300.0f; // 視界の奥行き
	static inline float kVisionWidth = 170.0f;  // 視界の幅
	static inline float kArriveDist = 16.0f;    // 経由点に到達したとみなす距離
	static inline float kTurnRate = 0.15f;      // 向きを変える滑らかさ(0〜1)
	static inline float kLoseTime = 1.5f;       // 見失ってから巡回に戻るまでの時間(秒)

	static inline uint32_t kBodyColor = 0xFF4455FF;   // 体の色(赤)
	static inline uint32_t kVisionColor = 0xFFEE5526; // 視界の色(巡回中・半透明の黄)
	static inline uint32_t kAlertColor = 0xFF3322AA;  // 視界の色(追跡中・半透明の赤)

	/// <summary>初期化。route = 巡回する経由点(2点以上)。</summary>
	void Initialize(const std::vector<Vector2>& route);

	/// <summary>スタート状態に戻す(捕まったとき用)。</summary>
	void Reset();

	/// <summary>更新。playerPos = プレイヤー中心座標。</summary>
	void Update(const Vector2& playerPos, const Stage& stage);

	/// <summary>描画(視界 → 体の順)。</summary>
	void Draw(const Vector2& viewOrigin, const std::wstring& windowTitle) const;

	/// <summary>いま視界にプレイヤーが入っているか。</summary>
	bool CanSeePlayer(const Vector2& playerPos, const Stage& stage) const;

	/// <summary>このフレームで「発見した瞬間」か(通知を1度だけ出す用)。</summary>
	bool JustFound() const { return justFound_; }

	/// <summary>プレイヤーに触れた(捕まえた)か。</summary>
	bool IsTouching(const Vector2& playerPos, float playerRadius) const;

	State GetState() const { return state_; }

private:
	// 最も近い経由点を次の目標にする(巡回復帰時に使う)
	void RetargetNearest();

	std::vector<Vector2> route_;    // 巡回ルート
	size_t targetIndex_ = 0;        // 次に向かう経由点
	Vector2 position_{};            // 現在位置
	Vector2 facing_ = {1.0f, 0.0f}; // 向き(単位ベクトル)
	State state_ = State::Patrol;   // 行動状態
	float loseTimer_ = 0.0f;        // 見失ってからの経過時間
	bool justFound_ = false;        // 発見した瞬間フラグ
};
