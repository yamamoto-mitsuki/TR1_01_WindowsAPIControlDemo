#pragma once
#include "Stage.h"
#include "MyEngine/Math/Vector2.h"
#include <cstdint>

class Player {
public:
    // 調整項目
	static inline Vector2 kInitPosition = {180.0f, 540.0f}; // スタート地点(ワールド座標)
	static inline float kSpeed = 260.0f;                    // 移動速度(ピクセル/秒)
	static inline float kRadius = 16.0f;                    // 当たり判定・見た目の半径
	static inline uint32_t kColor = 0xFFFFFFFF;             // 色(水色)

    void Initialize();
	void Update(const Stage& stage);
	void Draw(const Vector2& viewOrigin, const std::wstring& windowTitle) const;
	void Reset();

    // 調整項目を登録・適用
    static void RegisterGV();
    static void ApplyGV();

    // ゲッター
	const Vector2& GetPosition() const { return position_; }
	float GetRadius() const { return kRadius; }

private:
    Vector2 position_;
};