#include "ImGuiManager.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>
using namespace ImGui;

ImGuiManager* ImGuiManager::GetInstance()
{
	static ImGuiManager instance;

	return &instance;
}

void ImGuiManager::Initialize(WinApp* winApp, DXCommon* dxCom)
{
	HRESULT result;
	dxCom_ = dxCom;

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
		static_cast<uint32_t>(dxCom->GetBackBufferCount()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		srvHeap_.Get(),
		srvHeap_->GetCPUDescriptorHandleForHeapStart(),
		srvHeap_->GetGPUDescriptorHandleForHeapStart()
	);

	ImGuiIO& io = ImGui::GetIO();
	//フォント設定
	io.Fonts->AddFontDefault();
}

void ImGuiManager::Finalize()
{
	//後始末
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	DestroyContext();

	//デスクリプタヒープを解放
	srvHeap_.Reset();
}

void ImGuiManager::Begin()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	NewFrame();
}

void ImGuiManager::End()
{
	Render();
}

void ImGuiManager::Draw()
{
	ID3D12GraphicsCommandList* commandList = dxCom_->GetCommandList();

	//デスクリプタヒープの配列をセット
	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//描画コマンド
	ImGui_ImplDX12_RenderDrawData(GetDrawData(), commandList);
}
