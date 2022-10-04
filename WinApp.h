#pragma once
#include "Windows.h"

class WinApp
{
public:
	static const int WINDOW_WIDTH = 1280;	//横サイズ
	static const int WINDOW_HEIGHT = 720;	//縦サイズ

public:
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	void CreateWindowScreen();
	bool ProcMessage();
	void DeleteWindow();

public:
	WNDCLASSEX w{};
	RECT wrc;
	HWND hwnd;
	MSG msg{}; // メッセージ
};

