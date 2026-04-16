#pragma once
#include <Windows.h>
#include <string>

// ウィンドウサイズ
extern int32_t globalWindowWidth;
extern int32_t globalWindowHeight;

// ウィンドウプロシージャ
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
// ウィンドウ作成
HWND CreateMyWindow(HINSTANCE hInstance);