#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <assert.h>
#include <vector>
#include <wrl.h>

#include "GrovalSetting.h"
#include "WinApp.h"

class DXCommon
{
public:
	void Initialize(WinApp* winApp);

	ID3D12Device* GetDevice() {
		return device_.Get();
	}
	IDXGISwapChain4* GetSwapChain() {
		return swapChain_.Get();
	}
	ID3D12CommandAllocator* GetCommandAllocator() {
		return commandAllocator_.Get();
	}
	ID3D12GraphicsCommandList* GetCommandList() {
		return commandList_.Get();
	}
	ID3D12CommandQueue* GetCommandQueue() {
		return commandQueue_.Get();
	}
	ID3D12DescriptorHeap* GetRtvHeap() {
		return rtvHeap_.Get();
	}
	D3D12_DESCRIPTOR_HEAP_DESC GetRtvHeapDesc() {
		return rtvHeapDesc_;
	}
	ID3D12Resource* GetBackBuffers(UINT num) {
		return backBuffers_[num].Get();
	}
	ID3D12Fence* GetFence() {
		return fence_.Get();
	}
	UINT64 GetFanceVal() {
		return fenceVal_;
	}
	UINT64 AddGetFanceVal() {
		++fenceVal_;
		return fenceVal_;
	}

public:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
private:
	ComPtr<ID3D12Device> device_;
	ComPtr<IDXGIFactory7> dxgiFactory_;
	ComPtr<IDXGISwapChain4> swapChain_;
	ComPtr<ID3D12CommandAllocator> commandAllocator_;
	ComPtr<ID3D12GraphicsCommandList> commandList_;
	ComPtr<ID3D12CommandQueue> commandQueue_;
	ComPtr<ID3D12DescriptorHeap> rtvHeap_;
	// �A�_�v�^�[�̗񋓗p
	std::vector<ComPtr<IDXGIAdapter4>> adapters_;
	// �����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	ComPtr<IDXGIAdapter4> tmpAdapter_;
	D3D_FEATURE_LEVEL featureLevel_;
	//�R�}���h�L���[�̐ݒ�
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc_{};
	// �X���b�v�`�F�[���̐ݒ�
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};
	// �f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc_{};
	// �o�b�N�o�b�t�@
	std::vector<ComPtr<ID3D12Resource>> backBuffers_;
	// �t�F���X�̐���
	ComPtr<ID3D12Fence> fence_;
	UINT64 fenceVal_ = 0;
};

