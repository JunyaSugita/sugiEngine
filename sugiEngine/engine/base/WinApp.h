/**
 * @file WinApp.h
 * @brief ウィンドウ関係
 */

#pragma once
#include <Windows.h>
#include"GlobalSetting.h"

class WinApp
{
public:
	static LRESULT WindowProc(HWND hwnd, uint32_t msg, WPARAM wparam, LPARAM lparam);
	void CreateWindowScreen();
	bool ProcMessage();
	void DeleteWindow();

	HWND GetHWND() const { return hwnd_; }
	HINSTANCE GetHInstance() const { return w_.hInstance; }

private:
	WNDCLASSEX w_{};	
	HWND hwnd_;
	RECT wrc_;
	MSG msg_{};

};

