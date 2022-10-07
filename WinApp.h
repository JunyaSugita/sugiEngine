#pragma once
#include <Windows.h>

#include"GrovalSetting.h"

class WinApp
{
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

