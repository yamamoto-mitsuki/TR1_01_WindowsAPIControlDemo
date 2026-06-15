#pragma once
#include "MyEngine/Math/Vector2.h"
#include <cstdint>
#include <string>

//==========================================
// 2Dプリミティブ描画ヘルパー
//   DebugRender をラップして「塗りつぶし円(〇)」「塗りつぶし矩形(□)」を描く。
//   色は 0xRRGGBBAA 形式(例: 赤 = 0xFF0000FF)。
//==========================================
class ShapeRenderer {
public:
	/// <summary>
	/// 塗りつぶし円を描く。
	/// <para>2Dには円描画が無いので、中心から扇形(パイスライス)を並べて円を近似する。</para>
	/// </summary>
	/// <param name="center">中心座標(スクリーン座標・ピクセル)</param>
	/// <param name="radius">半径</param>
	/// <param name="color">色(0xRRGGBBAA)</param>
	/// <param name="windowTitle">描画先ウィンドウ</param>
	/// <param name="segments">分割数(多いほど滑らか)</param>
	static void DrawCircle(const Vector2& center, float radius, uint32_t color, const std::wstring& windowTitle, int segments = 24);

	/// <summary>
	/// 塗りつぶし矩形を描く(中心指定)。
	/// </summary>
	/// <param name="center">中心座標(スクリーン座標・ピクセル)</param>
	/// <param name="size">幅・高さ</param>
	/// <param name="color">色(0xRRGGBBAA)</param>
	/// <param name="windowTitle">描画先ウィンドウ</param>
	/// <param name="rotate">回転(ラジアン)</param>
	static void DrawRect(const Vector2& center, const Vector2& size, uint32_t color, const std::wstring& windowTitle, float rotate = 0.0f);
};
