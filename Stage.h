#pragma once
#include "MyEngine/Math/Vector2.h"
#include <cstdint>
#include <string>
#include <vector>

class Stage {
public:
    struct Box {
		Vector2 pos;
		Vector2 size;
    };

    void Initialize();

    /// <summary>
	/// 円を delta だけ動かす。壁にぶつかったら押し戻し、最終座標を返す。
	/// <para>X軸→Y軸の順に分けて解決する(壁ずりができて操作感が良い)。</para>
	/// </summary>
	Vector2 MoveAndCollide(const Vector2& pos, const Vector2& delta, float radius) const;

    /// <summary>点が壁の内側にあるか(視線判定用)。</summary>
	bool IsInsideWall(const Vector2& point) const;

	/// <summary>線分 a-b が壁で遮られているか(視線判定用)。</summary>
	bool IsSightBlocked(const Vector2& a, const Vector2& b) const;

	/// <summary>円がゴールに触れたか。</summary>
	bool ReachedGoal(const Vector2& pos, float radius) const;

    /// <summary>
    /// 描画
    /// </summary>
    /// <param name="viewOrigin">ウィンドウの左上ワールド座標</param>
    void Draw(const Vector2& viewOrigin, const std::wstring& windowTitle) const;

    const std::vector<Box>& GetWalls() const { return walls_; }
	const Box& GetGoal() const { return goal_; }

private:
	// 円(中心c, 半径r)と矩形boxが重なっているか
	static bool CircleHitsBox(const Vector2& c, float r, const Box& box);

	std::vector<Box> walls_;
	Box goal_{};
};