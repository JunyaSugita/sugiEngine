#include "ImGuiManager.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>
using namespace ImGui;

ImGuiManager::ImGuiManager()
{
}

ImGuiManager::~ImGuiManager()
{
}

ImGuiManager* ImGuiManager::GetInstance()
{
	static ImGuiManager instance;

	return &instance;
}

void ImGuiManager::Initialie(WinApp* winApp, DXCommon* dxCom)
{
	HRESULT result;
	dxCom_ = dxCom;

	//ImGui�̃R���e�L�X�g�𐶐�
	CreateContext();
	//ImGui�̃X�^�C��
	StyleColorsDark();

	ImGui_ImplWin32_Init(winApp->GetHWND());

	//�f�X�N���v�^�q�[�v
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	//�f�X�N���v�^�q�[�v����
	result = dxCom->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result));

	//DirectX12�p������
	ImGui_ImplDX12_Init(
		dxCom->GetDevice(),
		static_cast<uint32_t>(dxCom->GetBackBufferCount()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		srvHeap_.Get(),
		srvHeap_->GetCPUDescriptorHandleForHeapStart(),
		srvHeap_->GetGPUDescriptorHandleForHeapStart()
	);

	ImGuiIO& io = ImGui::GetIO();
	//�t�H���g�ݒ�
	io.Fonts->AddFontDefault();
}

void ImGuiManager::Finalize()
{
	//��n��
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	DestroyContext();

	//�f�X�N���v�^�q�[�v�����
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

	//�f�X�N���v�^�q�[�v�̔z����Z�b�g
	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//�`��R�}���h
	ImGui_ImplDX12_RenderDrawData(GetDrawData(), commandList);
}

void ImGuiManager::Text(const char* text, ...)
{
	ImGui::Text(text);
}
