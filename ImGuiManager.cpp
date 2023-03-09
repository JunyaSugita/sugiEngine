#include "ImGuiManager.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>
using namespace ImGui;


void ImGuiManager::Initialie(WinApp* winApp, DXCommon* dxCom)
{
	HRESULT result;

	//ImGuiのコンテキストを生成
	CreateContext();
	//ImGuiのスタイル
	StyleColorsDark();

	ImGui_ImplWin32_Init(winApp->GetHWND());

	//デスクリプタヒープ
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	//デスクリプタヒープ生成
	result = dxCom->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result));

	//DirectX12用初期化
	ImGui_ImplDX12_Init(
		dxCom->GetDevice(),
		static_cast<int>(dxCom->GetBackBufferCount()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		srvHeap_.Get(),
		srvHeap_->GetCPUDescriptorHandleForHeapStart(),
		srvHeap_->GetGPUDescriptorHandleForHeapStart()
	);

	ImGuiIO& io = ImGui::GetIO();
	//フォント設定
	io.Fonts->AddFontDefault();
}

void ImGuiManager::Text(const char* text, ...)
{
	ImGui::Text(text);
}
