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

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// ImGuiスタート
	/// </summary>
	void Begin();

	/// <summary>
	/// ImGui終わり
	/// </summary>
	void End();

	/// <summary>
	/// ImGui描画
	/// </summary>
	void Draw();

	void Text(const char* text,...);

private:
	//SRV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;
	DXCommon* dxCom_ = nullptr;
};

