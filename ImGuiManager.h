#pragma once
#include "WinApp.h"
#include "DXCommon.h"
#include <imgui.h>

class ImGuiManager
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="winApp">WinAppnのポインタ</param>
	void Initialie(WinApp* winApp,DXCommon* dxCom);

	void Text(const char* text,...);

private:
	//SRV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;

};

