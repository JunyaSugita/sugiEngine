#include "Particle.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <array>
#include "Camera.h"

using namespace Microsoft::WRL;
using namespace std;

ComPtr<ID3D12Device> Particle::sDevice = nullptr;
ComPtr<ID3D12PipelineState> Particle::sPipelineState = nullptr;
ComPtr<ID3D12RootSignature> Particle::sRootSignature;
ComPtr<ID3D12GraphicsCommandList> Particle::sCmdList;
std::array<ComPtr<ID3D12Resource>, Particle::MAX_SRV_COUNT> Particle::sTextureBuffers;
const size_t Particle::MAX_SRV_COUNT;
ComPtr<ID3D12DescriptorHeap> Particle::sSrvHeap;
uint32_t Particle::sIncrementSize;
uint32_t Particle::sTextureIndex = 0;

void Particle::StaticInitialize(ID3D12Device* device)
{
	HRESULT result;
	sDevice = device;

	ComPtr<ID3DBlob> vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> gsBlob = nullptr;
	ComPtr<ID3DBlob> psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob = nullptr; // �G���[�I�u�W�F�N�g
	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/Shaders/ParticleVS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);
	// �G���[�Ȃ�
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
	// �W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/Shaders/ParticleGS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "gs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&gsBlob, &errorBlob);
	// �G���[�Ȃ�
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/Shaders/ParticlePS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);
	// �G���[�Ȃ�
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
	{//xyz���W
		"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	}
	};

	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	// �V�F�[�_�[�̐ݒ�
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.GS.pShaderBytecode = gsBlob->GetBufferPointer();
	pipelineDesc.GS.BytecodeLength = gsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	// �T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�̐ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // �J�����O���Ȃ�
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����
	// �u�����h�X�e�[�g
	//pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
	//	= D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA�S�Ẵ`�����l����`��

	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	//RGBA�S�Ẵ`�����l����`��
	//���ʐݒ�(�A���t�@�l)
	blenddesc.BlendEnable = true;					//�u�����h��L���ɂ���
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		//�\�[�X�̒l��100%�g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//�f�X�g�̒l��0%�g��

	//����������
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;			//���Z
	blenddesc.SrcBlend = D3D12_BLEND_ONE;		//�\�[�X�̃A���t�@�l
	blenddesc.DestBlend = D3D12_BLEND_ONE;//1.0f-1�\�[�X�̃A���t�@�l

	// �u�����h�X�e�[�g�̐ݒ�
	for (int i = 0; i < MULTI_RENDAR_TARGET_NUM; i++) {
		pipelineDesc.BlendState.RenderTarget[i] = blenddesc;
	}

	// ���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);
	// �}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	// ���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 2; // �`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
	pipelineDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	//�f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;//t0
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//���[�g�p�����[�^
	D3D12_ROOT_PARAMETER rootParams[3] = {};
	//�萔�o�b�t�@0��
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//�萔�o�b�t�@�r���[
	rootParams[0].Descriptor.ShaderRegister = 0;					//�萔�o�b�t�@�ԍ�
	rootParams[0].Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[����
	//�e�N�X�`�����W�X�^
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//���
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;			//�f�X�N���v�^�����W
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//�f�X�N���v�^�����W��
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//�S�ẴV�F�[�_�[����
	//�萔�o�b�t�@1��
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//�萔�o�b�t�@�r���[
	rootParams[2].Descriptor.ShaderRegister = 1;					//�萔�o�b�t�@�ԍ�
	rootParams[2].Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	//�e�N�X�`���T���v���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���J��Ԃ�(�^�C�����O)
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//�c�J��Ԃ�(�^�C�����O)
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���s�J��Ԃ�(�^�C�����O)
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//�{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//�S�ă��j�A���
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//�~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;												//�~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//�s�N�Z���V�F�[�_����̂ݎg�p�\

	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;	//���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = _countof(rootParams);		//���[�g�p�����[�^��
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// ���[�g�V�O�l�`���̃V���A���C�Y
	ID3DBlob* rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&sRootSignature));
	assert(SUCCEEDED(result));
	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = sRootSignature.Get();

	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	pipelineDesc.DepthStencilState.DepthEnable = true;	//�[�x�e�X�g���s��
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;//�������݋���
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	//��������΍��i
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g

	// �p�C�v�����X�e�[�g�̐���
	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&sPipelineState));
	assert(SUCCEEDED(result));

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256�o�C�g�A���C�����g
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = MAX_SRV_COUNT;
	//�ݒ������SRV�p�̃f�X�N���v�^�q�[�v�𐶐�
	result = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&sSrvHeap));
	assert(SUCCEEDED(result));
}

void Particle::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	Particle::sCmdList = cmdList;
	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	cmdList->SetPipelineState(sPipelineState.Get());
	cmdList->SetGraphicsRootSignature(sRootSignature.Get());

	// �v���~�e�B�u�`��̐ݒ�R�}���h
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST); // �O�p�`���X�g
}

void Particle::PostDraw()
{
	Particle::sCmdList = nullptr;
}

uint32_t Particle::LoadTexture(const string& textureName) {
	HRESULT result;

	TexMetadata metadata{};
	ScratchImage scratchImg{};
	string fileName = "Resources/" + textureName;
	//���j�R�[�h������ɕϊ�����
	wchar_t wfilepath[128];
	MultiByteToWideChar(CP_ACP, 0, fileName.c_str(), -1, wfilepath, _countof(wfilepath));

	//WIC�e�N�X�`���̃��[�h
	result = LoadFromWICFile(
		wfilepath,
		WIC_FLAGS_NONE,
		&metadata,
		scratchImg
	);

	ScratchImage mipChain{};
	//�~�b�v�}�b�v����
	result = GenerateMipMaps(
		scratchImg.GetImages(),
		scratchImg.GetImageCount(),
		scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT,
		0,
		mipChain
	);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}
	//�ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = MakeSRGB(metadata.format);

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;
	textureResourceDesc.Height = (uint32_t)metadata.height;
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	//�e�N�X�`���o�b�t�@�̐���
	result = sDevice->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&sTextureBuffers[sTextureIndex])
	);

	//�S�~�b�v�}�b�v�ɂ���
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		//�~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = scratchImg.GetImage(i, 0, 0);
		// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = sTextureBuffers[sTextureIndex]->WriteToSubresource(
			(uint32_t)i,
			nullptr,
			img->pixels,
			(uint32_t)img->rowPitch,
			(uint32_t)img->slicePitch
		);
		assert(SUCCEEDED(result));
	}

	//�V�F�[�_�[���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//�ݒ�\����
	srvDesc.Format = textureResourceDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	//SRV�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = sSrvHeap->GetCPUDescriptorHandleForHeapStart();
	//�e�N�X�`���؂�ւ�
	sIncrementSize = sDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	srvHandle.ptr += sIncrementSize * sTextureIndex;

	//�n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	sDevice->CreateShaderResourceView(sTextureBuffers[sTextureIndex].Get(), &srvDesc, srvHandle);

	sTextureIndex++;

	return sTextureIndex - 1;
}

void Particle::Initialize(uint32_t texNum)
{
	HRESULT result;
	textureNum_ = texNum;
	AdjustTextureSize();

	size_ = textureSize_;

	uint32_t sizeVB = static_cast<uint32_t>(sizeof(vertices_[0]) * _countof(vertices_));

	// ���_�o�b�t�@�̐ݒ�
	heapProp_.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p
	// ���\�[�X�ݒ�
	resDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc_.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc_.Height = 1;
	resDesc_.DepthOrArraySize = 1;
	resDesc_.MipLevels = 1;
	resDesc_.SampleDesc.Count = 1;
	resDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// ���_�o�b�t�@�̐���
	result = sDevice->CreateCommittedResource(
		&heapProp_, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc_, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	VertexSp* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	for (int i = 0; i < vertexCount; i++) {
		vertMap[i] = vertices_[i]; // ���W���R�s�[
	}
	// �q���������
	vertBuff_->Unmap(0, nullptr);

	// GPU���z�A�h���X
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbView_.SizeInBytes = sizeVB;
	// ���_1���̃f�[�^�T�C�Y
	vbView_.StrideInBytes = sizeof(vertices_[0]);


	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;		//GPU�ւ̓]���p
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBuffB1) + 0xff) & ~0xff;	//256�o�C�g�A���C�����g
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;


	//�萔�o�b�t�@�̐���
	result = Particle::sDevice->CreateCommittedResource(
		&cbHeapProp,		//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform_)
	);
	assert(SUCCEEDED(result));

	//�萔�o�b�t�@�̃}�b�s���O
	result = constBuffTransform_->Map(0, nullptr, (void**)&constMapTransform_);	//�}�b�s���O
	assert(SUCCEEDED(result));

	//2D�̍s��

	worldTransform_.SetMatWorld(
		Matrix4(1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1)
	);
	worldTransform_.SetMatWorld(0, 0, 2.0f / WIN_WIDTH);
	worldTransform_.SetMatWorld(1, 1, -2.0f / WIN_HEIGHT);
	worldTransform_.SetMatWorld(3, 0, -1.0f);
	worldTransform_.SetMatWorld(3, 1, 1.0f);

	//���[���h�ϊ��s��
	WorldTransform matTransform;
	matTransform.SetMatWorld(
		Matrix4(1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1)
	);


	matTransform.SetRotZ(rotate_);
	matTransform.SetPos(Vector3(pos_.x, pos_.y, pos_.z));
	matTransform.SetWorldMat();

	//constMapTransform_->mat = matTransform.GetMatWorld() * worldTransform_.GetMatWorld();
	Camera* camera = Camera::GetInstance();
	constMapTransform_->viewproj = ConvertToXMMATRIX(camera->GetMatView() * camera->GetMatProjection());
	constMapTransform_->world = ConvertToXMMATRIX(matTransform.GetMatWorld());
	constMapTransform_->cameraPos = *camera->GetEyeXM();

	//�萔�o�b�t�@
	result = sDevice->CreateCommittedResource(
		&cbHeapProp,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial_)
	);
	assert(SUCCEEDED(result));

	result = constBuffMaterial_->Map(0, nullptr, (void**)&constMapMaterial_);	//�}�b�s���O
	assert(SUCCEEDED(result));

	constMapMaterial_->color = color_;

	SetUpVertex();
}

void Particle::Update()
{
	SetUpVertex();
}

void Particle::Draw()
{
	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	sCmdList->IASetVertexBuffers(0, 1, &vbView_);
	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	sCmdList->SetGraphicsRootConstantBufferView(0, constBuffMaterial_->GetGPUVirtualAddress());
	//�f�X�N���v�^�q�[�v�̔z����Z�b�g����R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = { sSrvHeap.Get() };
	sCmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//SRV�q�[�v�̐擪�n���h�����擾(SRV���w���͂�)
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = sSrvHeap->GetGPUDescriptorHandleForHeapStart();
	//�`�悷��e�N�X�`���̎w��
	srvGpuHandle.ptr += sIncrementSize * textureNum_;
	//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
	sCmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	sCmdList->SetGraphicsRootConstantBufferView(2, constBuffTransform_->GetGPUVirtualAddress());

	if (isView_ == true) {
		// �`��R�}���h
		sCmdList->DrawInstanced(vertexCount, 1, 0, 0); // �S�Ă̒��_���g���ĕ`��
	}
}

void Particle::SetPos(float x, float y) {
	pos_.x = x;
	pos_.y = y;

	SetUpVertex();
}

void Particle::SetRotate(float r) {
	rotate_ = r;

	SetUpVertex();
}

void Particle::SetColor(float x, float y, float z, float w) {
	color_.x = x;
	color_.y = y;
	color_.z = z;
	color_.w = w;

	SetUpVertex();
}

void Particle::SetSize(float x, float y) {
	size_.x = x;
	size_.y = y;

	SetUpVertex();
}

void Particle::SetAnchorPoint(float x, float y) {
	anchorPoint_.x = x;
	anchorPoint_.y = y;

	SetUpVertex();
}

void Particle::SetFlipX(bool isFlip) {
	isFlipX_ = isFlip;

	SetUpVertex();
}
void Particle::SetFlipY(bool isFlip) {
	isFlipY_ = isFlip;

	SetUpVertex();
}

void Particle::SetTextureSize(float x, float y) {
	textureSize_.x = x;
	textureSize_.y = y;

	SetUpVertex();
}

void Particle::Add(int life, XMFLOAT3 pos, XMFLOAT3 velo, XMFLOAT3 accel)
{
	particles_.emplace_front();
	PARTICLE& p = particles_.front();
	p.position = pos;
	p.velocity = velo;
	p.accel = accel;
	p.num_frame = life;
}

void Particle::SetUpVertex() {

	HRESULT result;

	float left = (0.0f - anchorPoint_.x) * size_.x;
	float right = (1.0f - anchorPoint_.x) * size_.x;
	float top = (0.0f - anchorPoint_.y) * size_.y;
	float bottom = (1.0f - anchorPoint_.y) * size_.y;

	if (isFlipX_ == true) {
		left *= -1;
		right *= -1;
	}
	if (isFlipY_ == true) {
		top *= -1;
		bottom *= -1;
	}

	//ID3D12Resource* textureBuffer = textureBuffers_[textureIndex].Get();
	if (sTextureBuffers[textureNum_]) {

		D3D12_RESOURCE_DESC resDesc = sTextureBuffers[textureNum_]->GetDesc();

		float tex_left = textureLeftTop_.x / resDesc.Width;
		float tex_right = (textureLeftTop_.x + textureSize_.x) / resDesc.Width;
		float tex_top = textureLeftTop_.y / resDesc.Height;
		float tex_bottom = (textureLeftTop_.y + textureSize_.y) / resDesc.Height;
	}

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	VertexSp* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	for (int i = 0; i < vertexCount; i++) {
		vertMap[i] = vertices_[i]; // ���W���R�s�[
	}
	// �q���������
	vertBuff_->Unmap(0, nullptr);

	//���[���h�ϊ��s��
	WorldTransform matTransform;
	matTransform.GetMatWorld().Initialize();
	matTransform.SetScale({ 10,10,10 });
	matTransform.SetRotZ(rotate_);
	matTransform.SetPos(Vector3(pos_.x, pos_.y, 0));
	matTransform.isBillboard = false;
	matTransform.SetWorldMat();

	//�r���{�[�h
	Camera* camera = Camera::GetInstance();

	constMapTransform_->viewproj = ConvertToXMMATRIX(camera->GetMatView()) * ConvertToXMMATRIX(camera->GetMatProjection());
	constMapTransform_->world = ConvertToXMMATRIX(matTransform.GetMatWorld());
	constMapTransform_->cameraPos = *camera->GetEyeXM();

	constMapMaterial_->color = color_;

}

void Particle::AdjustTextureSize()
{
	D3D12_RESOURCE_DESC resDesc = sTextureBuffers[textureNum_]->GetDesc();

	textureSize_.x = static_cast<float>(resDesc.Width);
	textureSize_.y = static_cast<float>(resDesc.Height);
}