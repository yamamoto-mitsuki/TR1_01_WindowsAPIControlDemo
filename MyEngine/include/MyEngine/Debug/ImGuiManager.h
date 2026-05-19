#ifdef USE_IMGUI
#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <functional>
#include <string>
#include <vector>
#include "MyEngine/Math/Vector2.h"
#include "MyEngine/Window/Win32Window.h"

// 前方宣言
class DirectXCommon;

class ImGuiManager {
public:
	enum class ImGuiRegion {
		Right,
		Left,
		Bottom,

		Count // 参照用（要素数取得のためのものなので使用してはいけない）
	};

	static ImGuiManager* GetInstance();

	/// <summary>
	/// 終了処理。WinMainのreturn 0の前に必ず呼ぶ
	/// </summary>
	void Finalize();

	/// <summary>
	///  初期化
	/// </summary>
	/// <param name="window">表示したいウィンドウ</param>
	/// <param name="dxCommon">DirectX環境</param>
	void Init(Win32Window* window, DirectXCommon* dxCommon, const WindowConfig& config);

	/// <summary>
	/// フレームの開始処理
	/// </summary>
	void Begin();

	/// <summary>
	/// 描画リクエストを追加する
	/// </summary>
	/// <param name="region">画面の上、下、右、左どこに描画するか指定</param>
	/// <param name="pos">4方向選んだとこのどこから描画するか</param>
	/// <param name="size">ImGuiウィンドウのサイズ（0で領域全体）</param>
	/// <param name="drawFunc">ImGui::Begin/Endなどを書いたラムダ</param>
	static void AddDrawRequest(ImGuiRegion region, const Vector2& pos, const Vector2& size, std::function<void()> drawFunc);

	/// <summary>
	/// 描画データをコマンドリストに積む
	/// </summary>
	void ProcessRequests();

	/// <summary>
	/// 描画前準備
	/// </summary>
	void Render();

	/// <summary>
	/// 全ウィンドウの描画コマンドを実行
	/// </summary>
	void RenderDrawData();
	
	/// <summary>
	/// 全リクエストをクリア（PostRenderAllの末尾で呼ぶ）
	/// </summary>
	static void ClearRequests();

private:
	// コピー・ムーブ禁止
	ImGuiManager() = default;
	~ImGuiManager() = default;

	// 描画リクエストの構造体
	struct DrawRequest {
		ImGuiRegion region; // 画面のどこに配置するか
		Vector2 pos;   // 領域内の相対座標
		Vector2 size;  // 0=領域全体
		std::function<void()> drawFunc;
	};

	// ImGuiウィンドウの領域
	struct RegionRect {
		float x = 0.0f;
		float y = 0.0f;
		float w = 0.0f;
		float h = 0.0f;
	};

	/// <summary>
	/// ウィンドウサイズとWindowConfigからregionごとの描画領域を計算する
	/// </summary>
	std::unordered_map<ImGuiRegion, RegionRect> CalcRegionRects(float totalWidth, float totalHeight) const;

	DirectXCommon* dxCommon_ = nullptr;
	HWND hwnd_ = nullptr;
	WindowConfig wndConfig_;

	std::vector<DrawRequest> requests_;
};
#endif