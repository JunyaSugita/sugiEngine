#include "WinApp.h"
#pragma comment(lib,"winmm.lib")
#include <imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, uint32_t msg, WPARAM wparam, LPARAM lparam);

LRESULT WinApp::WindowProc(HWND hwnd, uint32_t msg, WPARAM wparam, LPARAM lparam)
{
	//ImGui用ウィンドウプロシージャ
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
		return true;
	}

	//メッセージに大路手ゲーム固有の処理を行う
	switch (msg)
	{
		//ウィンドウが破壊された
	case WM_DESTROY:
		//OSに対してアプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	//標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void WinApp::CreateWindowScreen()
{
	//ウィンドウクラスの設定
	w_.cbSize = sizeof(WNDCLASSEX);
	w_.lpfnWndProc = (WNDPROC)WindowProc; // ウィンドウプロシージャを設定
	w_.lpszClassName = L"DirectX"; // ウィンドウクラス名
	w_.hInstance = GetModuleHandle(nullptr); // ウィンドウハンドル
	w_.hCursor = LoadCursor(NULL, IDC_ARROW); // カーソル指定

	// ウィンドウクラスをOSに登録する
	RegisterClassEx(&w_);

	// ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
	wrc_ = { 0, 0, WIN_WIDTH, WIN_HEIGHT };
	// 自動でサイズを補正する
	AdjustWindowRect(&wrc_, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウオブジェクトの生成
	hwnd_ = CreateWindow(w_.lpszClassName, // クラス名
		L"Darkness v0.4.0", // タイトルバーの文字
		WS_OVERLAPPEDWINDOW, // 標準的なウィンドウスタイル
		CW_USEDEFAULT, // 表示X座標(OSに任せる)
		CW_USEDEFAULT, // 表示Y座標(OSに任せる)
		wrc_.right - wrc_.left, // ウィンドウ横幅
		wrc_.bottom - wrc_.top, // ウィンドウ縦幅
		nullptr, // 親ウィンドウハンドル
		nullptr, // メニューハンドル
		w_.hInstance, // 呼び出しアプリケーションハンドル
		nullptr); // オプション
	// ウィンドウを表示状態にする
	ShowWindow(hwnd_, SW_SHOW);

	//システムタイマーの分解度をあげる
	timeBeginPeriod(1);
}

bool WinApp::ProcMessage()
{
	// メッセージがある?
	if (PeekMessage(&msg_, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg_); // キー入力メッセージの処理
		DispatchMessage(&msg_); // プロシージャにメッセージを送る
	}
	// ✖ボタンで終了メッセージが来たらゲームループを抜ける
	if (msg_.message == WM_QUIT) {
		return true;
	}

	return false;
}

void WinApp::DeleteWindow()
{
	// ウィンドウクラスを登録解除
	UnregisterClass(w_.lpszClassName, w_.hInstance);
}