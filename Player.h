#pragma once
#include "MyEngine/Math/Vector2.h"
#include <cstdint>

class Player {
public:
    // 調整項目
    static inline Vector2 kInitPosition = { 640.0f,600.0f };
    static inline float kSpeed = 3.0f;
    static inline Vector2 kSize = { 30.0f,30.0f };

    // ゴールの種類
    enum class GoalState {
        Not,  // まだゴールしていない
        Goal, // ゴールした
    };


    void Initialize();
    void Update();
    void Draw();

    // 調整項目を登録・適用
    static void RegisterGV();
    static void ApplyGV();

    // ゲッター
    GoalState GetGoalState() const { return goalState_; }

private:
    Vector2 position_;
    Vector2 size_;
    uint32_t color_;

    GoalState goalState_;
};