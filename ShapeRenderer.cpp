#include "ShapeRenderer.h"
#include "MyEngine/Render/DebugRender.h"
#include <cmath>

//==========================================
// 塗りつぶし円
//==========================================
void ShapeRenderer::DrawCircle(const Vector2& center, float radius, uint32_t color, const std::wstring& windowTitle, int segments) {
	constexpr float kTwoPi = 6.28318530718f;
	if (segments < 3) {
		segments = 3;
	}

	for (int i = 0; i < segments; ++i) {
		float a0 = kTwoPi * static_cast<float>(i) / static_cast<float>(segments);
		float a1 = kTwoPi * static_cast<float>(i + 1) / static_cast<float>(segments);

		// 円周上の2点
		Vector2 p0 = {center.x + std::cos(a0) * radius, center.y + std::sin(a0) * radius};
		Vector2 p1 = {center.x + std::cos(a1) * radius, center.y + std::sin(a1) * radius};

		// 扇形1枚(中心・p0・p1の三角形)を Quad2d で表現する。
		// Quad2dは lb,lt,rb,rt の順で頂点を持ち、三角形(lb,lt,rb)が描かれる。
		// この三角形が表向き(時計回り)になるよう頂点を割り当てる。
		// 2DのPSOは裏面カリングが有効なので、巻き順を間違えると描画されない。
		// 念のため両方の巻き順を出して、どちらの環境でも必ず塗られるようにする。
		DebugRender::Quad2dConfig q;
		q.color = color;
		q.windowTitle = windowTitle;

		// 表向き想定
		q.lb = p1;
		q.lt = center;
		q.rb = p0;
		q.rt = center;
		DebugRender::DrawQuad2d(q);

		// 逆向き(保険)
		q.lb = p0;
		q.lt = center;
		q.rb = p1;
		q.rt = center;
		DebugRender::DrawQuad2d(q);
	}
}

//==========================================
// 塗りつぶし矩形
//==========================================
void ShapeRenderer::DrawRect(const Vector2& center, const Vector2& size, uint32_t color, const std::wstring& windowTitle, float rotate) {
	DebugRender::Rect2dConfig r;
	r.position = center;
	r.size = size;
	r.color = color;
	r.rotate = rotate;
	r.windowTitle = windowTitle;
	DebugRender::DrawRect2d(r);
}
