#include "Fbx.h"
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

ID3D12Device* Fbx::device_ = nullptr;
Camera* Fbx::camera_ = nullptr;
ComPtr<ID3D12RootSignature> Fbx::sRootSignature;
ComPtr<ID3D12PipelineState> Fbx::sPipelineState;
ID3D12GraphicsCommandList* Fbx::sCmdList = nullptr;

void Fbx::CreateGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;    // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g

	assert(device_);

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/FBXVS.hlsl",    // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0",    // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/FBXPS.hlsl",    // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0",    // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy���W
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // �@���x�N�g��
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv���W
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ //�e���̎󂯂�{�[���ԍ�
			"BONEINDICES",0,DXGI_FORMAT_R32G32B32A32_UINT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"BONEWEIGHTS",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		}
	};

	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	gpipeline.VS.BytecodeLength = vsBlob->GetBufferSize();
	gpipeline.PS.pShaderBytecode = psBlob->GetBufferPointer();
	gpipeline.PS.BytecodeLength = psBlob->GetBufferSize();

	// �T���v���}�X�N
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; // �w�ʂ��J�����O
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
	gpipeline.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

	// �f�v�X�X�e���V���X�e�[�g
	gpipeline.DepthStencilState.DepthEnable = true;	//�[�x�e�X�g���s��
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//�������݋���
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	//��������΍��i

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipeline.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;    // RBGA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ�
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�i�O�p�`�j
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;    // �`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// �f�X�N���v�^�����W
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// ���[�g�p�����[�^
	D3D12_ROOT_PARAMETER rootParams[3] = {};
	// CBV�i���W�ϊ��s��p�j
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//���
	rootParams[0].Descriptor.ShaderRegister = 0;					//�萔�o�b�t�@�ԍ�
	rootParams[0].Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����
	// SRV�i�e�N�X�`���j
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//���
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;					//�萔�o�b�t�@�ԍ�
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//�f�t�H���g�l
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����
	// CBV (�X�L�j���O�p)
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//���
	rootParams[2].Descriptor.ShaderRegister = 3;					//�萔�o�b�t�@�ԍ�
	rootParams[2].Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����

	// �X�^�e�B�b�N�T���v���[
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���J��Ԃ�(�^�C�����O)
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//�c�J��Ԃ�(�^�C�����O)
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���s���J��Ԃ�(�^�C�����O)
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//�{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//�S�ă��j�A���
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//�~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;												//�~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//�s�N�Z���V�F�[�_����̂ݎg�p�\


	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;	//���[�g�p�����[�^�[�̐擪�A�h���X
	rootSignatureDesc.NumParameters = _countof(rootParams);		//�p�����[�^�[��
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	ComPtr<ID3DBlob> rootSigBlob;
	// �o�[�W������������̃V���A���C�Y
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ���[�g�V�O�l�`���̐���
	result = device_->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(sRootSignature.ReleaseAndGetAddressOf()));
	if (FAILED(result)) {
		assert(0);
	}

	gpipeline.pRootSignature = sRootSignature.Get();

	// �O���t�B�b�N�X�p�C�v���C���̐���
	result = device_->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(sPipelineState.ReleaseAndGetAddressOf()));
	if (FAILED(result)) {
		assert(0);
	}

}

void Fbx::Initialize()
{
	HRESULT result;

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;		//GPU�ւ̓]���p
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;	//256�o�C�g�A���C�����g
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	result = device_->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform_)
	);

	//�X�L�j���O���萔�o�b�t�@
	cbResourceDesc.Width = (sizeof(ConstBufferDataSkin) + 0xff) & ~0xff;

	result = device_->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffSkin_)
	);

	//�X�L������
	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin_->Map(0, nullptr, (void**)&constMapSkin);
	for (int i = 0; i < MAX_BONES; i++) {
		constMapSkin->bones[i] = XMMatrixIdentity();
	}
	constBuffSkin_->Unmap(0, nullptr);

	//1�t���[�����̎��Ԃ�60FPS�Őݒ�
	frameTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);

}

void Fbx::Update()
{
	worldTransform_.SetScale(scale_);
	worldTransform_.SetPos(position_);
	worldTransform_.SetRot(rotation_);
	worldTransform_.SetWorldMat();

	const XMMATRIX& modelTransform = model_->GetModelTransform();


	Camera* camera = Camera::GetInstance();

	HRESULT result;

	ConstBufferDataTransform* constMap = nullptr;
	result = constBuffTransform_->Map(0, nullptr, (void**)&constMap);

	constMap->viewproj = ConvertToXMMATRIX(camera->GetMatView() * camera->GetMatProjection());
	constMap->world = ConvertToXMMATRIX(worldTransform_.GetMatWorld());
	constMap->cameraPos = *camera->GetEyeXM();
	constBuffTransform_->Unmap(0, nullptr);

	std::vector<FbxModel::Bone>& bones = model_->GetBones();

	//�A�j���V����
	if (isPlay) {
		currentTime += frameTime;
		if (currentTime > endTime) {
			currentTime = startTime;
		}
	}

	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin_->Map(0, nullptr, (void**)&constMapSkin);
	for (int i = 0; i < bones.size(); i++) {
		XMMATRIX matCurrentPose;
		FbxAMatrix fbxCurrentPose = bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);
		FbxLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);
		//�������ăX�L�j���O�s���
		constMapSkin->bones[i] = bones[i].invInitializePose * matCurrentPose;
	}
	constBuffSkin_->Unmap(0, nullptr);
}

void Fbx::Draw()
{
	if (model_ == nullptr) {
		return;
	}

	sCmdList->SetGraphicsRootConstantBufferView(0, constBuffTransform_->GetGPUVirtualAddress());
	sCmdList->SetGraphicsRootConstantBufferView(2, constBuffSkin_->GetGPUVirtualAddress());

	model_->Draw(sCmdList);
}

void Fbx::PlayAnimation()
{
	FbxScene* fbxScene = model_->GetFbxScene();

	FbxAnimStack* animstack = fbxScene->GetSrcObject<FbxAnimStack>(0);
	const char* animstackname = animstack->GetName();
	FbxTakeInfo* takeinfo = fbxScene->GetTakeInfo(animstackname);

	startTime = takeinfo->mLocalTimeSpan.GetStart();
	endTime = takeinfo->mLocalTimeSpan.GetStop();
	currentTime = startTime;
	isPlay = true;
}

void Fbx::PreDraw(ID3D12GraphicsCommandList* cmdList_)
{
	sCmdList = cmdList_;
	sCmdList->SetPipelineState(sPipelineState.Get());
	sCmdList->SetGraphicsRootSignature(sRootSignature.Get());
	sCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Fbx::PostDraw()
{
	sCmdList = nullptr;
}
