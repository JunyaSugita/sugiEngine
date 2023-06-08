#include "DXCommon.h"

void DXCommon::Initialize(WinApp* winApp)
{
	HRESULT result;

#ifdef _DEBUG
	//�f�o�b�O���C���[���I����
	ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(true);
	}
#endif
	//FPS�Œ菉����
	InitializeFixFPS();

	// DXGI�t�@�N�g���[�̐���
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));
	assert(SUCCEEDED(result));

	// �p�t�H�[�}���X���������̂��珇�ɁA�S�ẴA�_�v�^�[��񋓂���
	for (UINT i = 0;
		dxgiFactory_->EnumAdapterByGpuPreference(i,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&tmpAdapter_)) != DXGI_ERROR_NOT_FOUND;
		i++) {
		// ���I�z��ɒǉ�����
		adapters_.push_back(tmpAdapter_);
	}

	// �Ó��ȃA�_�v�^��I�ʂ���
	for (size_t i = 0; i < adapters_.size(); i++) {
		DXGI_ADAPTER_DESC3 adapterDesc;
		// �A�_�v�^�[�̏����擾����
		adapters_[i]->GetDesc3(&adapterDesc);
		// �\�t�g�E�F�A�f�o�C�X�����
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			// �f�o�C�X���̗p���ă��[�v�𔲂���
			tmpAdapter_ = adapters_[i];
			break;
		}
	}

	// �Ή����x���̔z��
	D3D_FEATURE_LEVEL levels[] = {
	D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0,
	};

	for (size_t i = 0; i < _countof(levels); i++) {
		// �̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
		result = D3D12CreateDevice(tmpAdapter_.Get(), levels[i],
			IID_PPV_ARGS(&device_));
		if (result == S_OK) {
			// �f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
			featureLevel_ = levels[i];
			break;
		}
	}

#ifdef _DEBUG

	ComPtr<ID3D12InfoQueue> infoQueue;
	if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
	}

#endif

	// �R�}���h�A���P�[�^�𐶐�
	result = device_->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&commandAllocator_));
	assert(SUCCEEDED(result));
	// �R�}���h���X�g�𐶐�
	result = device_->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator_.Get(), nullptr,
		IID_PPV_ARGS(&commandList_));
	assert(SUCCEEDED(result));

	//�R�}���h�L���[�𐶐�
	result = device_->CreateCommandQueue(&commandQueueDesc_, IID_PPV_ARGS(&commandQueue_));
	assert(SUCCEEDED(result));

	// �X���b�v�`�F�[���̐ݒ�
	swapChainDesc_.Width = WIN_WIDTH;
	swapChainDesc_.Height = WIN_HEIGHT;
	swapChainDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // �F���̏���
	swapChainDesc_.SampleDesc.Count = 1; // �}���`�T���v�����Ȃ�
	swapChainDesc_.BufferUsage = DXGI_USAGE_BACK_BUFFER; // �o�b�N�o�b�t�@�p
	swapChainDesc_.BufferCount = 2; // �o�b�t�@����2�ɐݒ�
	swapChainDesc_.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // �t���b�v��͔j��
	swapChainDesc_.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	//ComPtr�̗p��
	ComPtr<IDXGISwapChain1> swapChain1;

	// �X���b�v�`�F�[���̐���
	result = dxgiFactory_->CreateSwapChainForHwnd(
		commandQueue_.Get(), winApp->GetHWND(), &swapChainDesc_, nullptr, nullptr,
		&swapChain1);
	assert(SUCCEEDED(result));
	//��������SwapChain1��4�ɕϊ�
	swapChain1.As(&swapChain_);

	// �f�X�N���v�^�q�[�v�̐ݒ�
	rtvHeapDesc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // �����_�[�^�[�Q�b�g�r���[
	rtvHeapDesc_.NumDescriptors = swapChainDesc_.BufferCount; // ���\��2��

	// �f�X�N���v�^�q�[�v�̐���
	device_->CreateDescriptorHeap(&rtvHeapDesc_, IID_PPV_ARGS(&rtvHeap_));

	// �o�b�N�o�b�t�@
	backBuffers_.resize(swapChainDesc_.BufferCount);

	// �X���b�v�`�F�[���̑S�Ẵo�b�t�@�ɂ��ď�������
	for (size_t i = 0; i < backBuffers_.size(); i++) {
		// �X���b�v�`�F�[������o�b�t�@���擾
		swapChain_->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers_[i]));
		// �f�X�N���v�^�q�[�v�̃n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap_->GetCPUDescriptorHandleForHeapStart();
		// �����\���ŃA�h���X�������
		rtvHandle.ptr += i * device_->GetDescriptorHandleIncrementSize(rtvHeapDesc_.Type);
		// �����_�[�^�[�Q�b�g�r���[�̐ݒ�
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		// �V�F�[�_�[�̌v�Z���ʂ�SRGB�ɕϊ����ď�������
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		// �����_�[�^�[�Q�b�g�r���[�̐���
		device_->CreateRenderTargetView(backBuffers_[i].Get(), &rtvDesc, rtvHandle);
	}

	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = WIN_WIDTH;
	depthResourceDesc.Height = WIN_HEIGHT;
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	//�[�x�l�p�q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	//�[�x�l�̃N���A�ݒ�
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;		//�[�x�l1.0f(�ő�l)�ŃN���A
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g
	//���\�[�X����
	result = GetDevice()->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,	//�[�x�l�������݂Ɏg�p
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff_)
	);
	//�[�x�r���[�p�f�X�N���v�^�q�[�v����
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;//�[�x�r���[1��
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;//�f�v�X�X�e���V���r���[
	result = GetDevice()->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap_));

	//�[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	GetDevice()->CreateDepthStencilView(
		depthBuff_.Get(),
		&dsvDesc,
		dsvHeap_->GetCPUDescriptorHandleForHeapStart()
	);

	// �t�F���X�̐���
	result = device_->CreateFence(fenceVal_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
}

void DXCommon::PreDraw()
{
	// �o�b�N�o�b�t�@�̔ԍ����擾(2�Ȃ̂�0�Ԃ�1��)
	UINT bbIndex = GetSwapChain()->GetCurrentBackBufferIndex();
	// 1.���\�[�X�o���A�ŏ������݉\�ɕύX
	barrierDesc_.Transition.pResource = GetBackBuffers(bbIndex); // �o�b�N�o�b�t�@���w��
	barrierDesc_.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // �\����Ԃ���
	barrierDesc_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // �`���Ԃ�
	GetCommandList()->ResourceBarrier(1, &barrierDesc_);
	// 2.�`���̕ύX
	// �����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = GetRtvHeap()->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += bbIndex * GetDevice()->GetDescriptorHandleIncrementSize(GetRtvHeapDesc().Type);
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap_->GetCPUDescriptorHandleForHeapStart();
	GetCommandList()->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);
	// 3.��ʃN���A R G B A
	FLOAT clearColor[] = { 0.1f,0.25f, 0.5f,0.0f }; // ���ۂ��F
	GetCommandList()->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	GetCommandList()->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	// 4.�`��R�}���h 
	// �r���[�|�[�g�ݒ�R�}���h
	D3D12_VIEWPORT viewport{};
	viewport.Width = WIN_WIDTH;
	viewport.Height = WIN_HEIGHT;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	// �r���[�|�[�g�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	GetCommandList()->RSSetViewports(1, &viewport);

	// �V�U�[��`
	D3D12_RECT scissorRect{};
	scissorRect.left = 0; // �؂蔲�����W��
	scissorRect.right = scissorRect.left + WIN_WIDTH; // �؂蔲�����W�E
	scissorRect.top = 0; // �؂蔲�����W��
	scissorRect.bottom = scissorRect.top + WIN_HEIGHT; // �؂蔲�����W��
	// �V�U�[��`�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	GetCommandList()->RSSetScissorRects(1, &scissorRect);

}

void DXCommon::PostDraw()
{
	HRESULT result;

	// 5.���\�[�X�o���A��߂�
	barrierDesc_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // �`���Ԃ���
	barrierDesc_.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT; // �\����Ԃ�
	GetCommandList()->ResourceBarrier(1, &barrierDesc_);

	// ���߂̃N���[�Y
	result = GetCommandList()->Close();
	assert(SUCCEEDED(result));
	// �R�}���h���X�g�̎��s
	ComPtr<ID3D12CommandList> commandLists[] = { GetCommandList() };
	GetCommandQueue()->ExecuteCommandLists(1, commandLists->GetAddressOf());
	// ��ʂɕ\������o�b�t�@���t���b�v(���\�̓��ւ�)
	result = GetSwapChain()->Present(1, 0);
	assert(SUCCEEDED(result));

	// �R�}���h�̎��s������҂�
	GetCommandQueue()->Signal(GetFence(), AddGetFanceVal());
	if (GetFence()->GetCompletedValue() != GetFanceVal()) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		GetFence()->SetEventOnCompletion(GetFanceVal(), event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	//FPS�Œ�
	UpdateFixFPS();

	// �L���[���N���A
	result = GetCommandAllocator()->Reset();
	assert(SUCCEEDED(result));
	// �ĂуR�}���h���X�g�𒙂߂鏀��
	result = GetCommandList()->Reset(GetCommandAllocator(), nullptr);
	assert(SUCCEEDED(result));
}

void DXCommon::InitializeFixFPS()
{
	//���ݎ��Ԃ��L�^����
	reference_ = std::chrono::steady_clock::now();
}

void DXCommon::UpdateFixFPS()
{
	// 1/60�b�҂�����̎���
	const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));
	// 1/60�b���킸���ɒZ������
	const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 65.0f));

	//���ݎ������擾
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

	//�O��L�^����̌o�ߎ��Ԃ��擾
	std::chrono::microseconds elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	// 1/60�b(���킸���Ȏ���)�o���Ă��Ȃ��ꍇ
	if (elapsed < kMinCheckTime) {
		while (std::chrono::steady_clock::now() - reference_ < kMinTime)
		{
			//1�}�C�N���b�X���[�v
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}

	//���ݎ������擾
	reference_ = std::chrono::steady_clock::now();
}
