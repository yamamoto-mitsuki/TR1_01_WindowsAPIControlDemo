#pragma once
#include "MyEngine/Math/Vector2.h"
#include "MyEngine/Window/Win32Window.h"
#include <d3d12.h>
#include <functional>
#include <string>
#include <vector>
#include <cassert>
#include <map>
#include <utility>
#include <wrl.h>

#ifdef USE_IMGUI
// デバック表示のカテゴリ
enum class DebugCategory {
	Performance, // 時間系
	Rendering,   // 描画系
	Memory,      // メモリ
};


/// <summary>
/// ImGuiの描画を管理するクラス。
/// </summary>
class ImGuiManager {
public:
	ImGuiManager(const ImGuiManager&) = delete;
	ImGuiManager& operator=(const ImGuiManager&) = delete;

	/// <summary>
	/// 初期化。Engine::Initialize() 内で1度だけ呼ぶ。
	/// </summary>
	static void Initialize(Win32Window* window);

	/// <summary>
	/// 解放
	/// </summary>
	static void Release();

	/// <summary>
	/// フレームの開始処理。毎フレーム最初に呼ぶ。
	/// </summary>
	static void Begin();

	/// <summary>
	/// ImGuiウィンドウの描画リクエストを追加する。
	/// </summary>
	/// <param name="drawFunc">ImGui::Begin / End を含むラムダ式</param>
	static void AddDrawRequest(std::function<void()> drawFunc);

	/// <summary>
	/// AddDrawRequest() で積まれた描画リクエストを全て実行する。
	/// </summary>
	static void ProcessRequests();

	/// <summary>
	/// 描画データを確定させる（ImGui::Render() のラッパー）。
	/// </summary>
	static void Render();

	/// <summary>
	/// 確定した描画データをコマンドリストに積む。
	/// </summary>
	static void RenderDrawData();

	/// <summary>
	/// 全リクエストをクリアする。
	/// </summary>
	static void ClearRequests();

	 /// <summary>
	/// デバッグオーバーレイに値をセットする。
	/// <para>同じ category + label がすでに登録済みなら値を上書きする。毎フレーム呼んでよい。</para>
	/// </summary>
	/// <param name="category">表示カテゴリ（enum順に並ぶ）</param>
	/// <param name="label">項目名（カテゴリ内での識別キーにもなる）</param>
	/// <param name="value">表示する値。int / float / double を渡せる</param>
	/// <param name="unit">単位。省略可（例: "ms" "GB" "MB"）</param>
	template<typename T> 
	static void SetDebugValue(DebugCategory category, const std::string& label, T value, const std::string& unit = "") {
		static_assert(std::is_arithmetic_v<T>, "SetDebugValue: int / float / double のみ渡せます");
		assert(instance_ && "[ImGuiManager::SetDebugValue] Initialize()を先に呼んでください");
		// float/doubleは小数点2桁、intは整数で文字列化
		std::string str;
		if constexpr (std::is_floating_point_v<T>) {
			char buf[32];
			snprintf(buf, sizeof(buf), "%.2f", static_cast<double>(value));
			str = buf;
		} else {
			str = std::to_string(static_cast<long long>(value));
		}
		if (!unit.empty()) {
			str += " " + unit;
		}

		// 登録済みなら上書き、なければ追加
		auto& entries = instance_->debugEntries_[category];
		for (auto& e : entries) {
			if (e.first == label) {
				e.second = str;
				return;
			}
		}
		entries.emplace_back(label, str);
	}

private:
	static ImGuiManager* instance_;

	ImGuiManager() = default;
	~ImGuiManager() = default;

	/// <summary>
	/// 現在のImGuiStyleを imgui_style.ini に書き出す。
	/// <para>色・Alpha・サイズ系パラメータを全て保存する。</para>
	/// </summary>
	/// <param name="path">保存先ファイルパス</param>
	void SaveStyle(const std::string& path);

	/// <summary>
	/// imgui_style.ini からImGuiStyleを読み込む。
	/// <para>ファイルが存在しない場合はスキップする（初回起動時は正常動作）</para>
	/// </summary>
	/// <param name="path">読み込み元ファイルパス</param>
	void LoadStyle(const std::string& path);

	/// <summary>
	/// デバッグオーバーレイウィンドウを描画する。
	/// </summary>
	void DrawDebugOverlay();

	// 描画リクエストのキュー
	std::vector<std::function<void()>> requests_;
	// メインウィンドウのハンドル
	HWND hwnd_ = nullptr;

	// Setting メニューの表示状態フラグ
	bool isShowStyleEditor_ = false;

	// デバッグオーバーレイのデータ
	// key   : DebugCategory（enum値の昇順で表示）
	// value : label と 値文字列 のペアリスト（登録順に表示）
	std::map<DebugCategory, std::vector<std::pair<std::string, std::string>>> debugEntries_;
};

#else

/// <summary>
/// USE_IMGUI が定義されていない場合の何もしないスタブ。
/// </summary>
class ImGuiManager {
public:
	static void Initialize(Win32Window*, const WindowConfig&) {}
	static void Release() {}
	static void Begin() {}
	static void Render() {}
	static void RenderDrawData() {}
	static void ClearRequests() {}
	static void ProcessRequests() {}
	static void AddDrawRequest(std::function<void()>) {}
};

#endif