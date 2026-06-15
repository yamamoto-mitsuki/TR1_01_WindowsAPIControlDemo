#pragma once
#include "MyEngine/Math/Vector2.h"
#include <vector>
#include <string>

// 前方宣言
class Player;
class Win32Window;

// ウィンドウ制御クラス
class WindowController {
public:
	void Initialize(Player* player);
	void Update();
	
    /// <summary>クリアしたら true を渡す。×ボタンで閉じられるようになる。</summary>
    void SetCanClose(bool canClose) { canClose_ = canClose; }
	
    /// <summary>ビュー(ウィンドウ)左上のワールド座標。描画の原点に使う。</summary>
	const Vector2& GetViewOrigin() const { return viewOrigin_; }
    
    /// <summary>ビュー(ウィンドウ)の表示サイズ。</summary>
	const Vector2& GetViewSize() const { return viewSize_; }

private:
	// 閉じる制御のフック(onCanClose_ / onTryClose_)を1度だけ仕込む
	void SetupCloseLock();
	// プレイヤーを追ってウィンドウを移動し、viewOrigin_ を更新する
	void UpdateViewport();

	Player* player_ = nullptr;
	Win32Window* window_ = nullptr;
	Vector2 viewOrigin_ = {0.0f, 0.0f}; // ビュー左上のワールド座標
	Vector2 viewSize_ = {0.0f, 0.0f};   // ビューの表示サイズ
	bool canClose_ = false;             // クリアしたか(閉じられるか)
	bool closeHooked_ = false;          // 閉じる制御を仕込んだか
};