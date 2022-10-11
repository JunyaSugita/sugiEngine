#include "Object3d.h"

void Object3d::Initialize(DXCommon* dxCom)
{
	this->dxCom = dxCom;
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

	//�萔�o�b�t�@�̐���
	result = dxCom->device->CreateCommittedResource(
		&cbHeapProp,		//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform)
	);
	assert(SUCCEEDED(result));

	//�萔�o�b�t�@�̃}�b�s���O
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);	//�}�b�s���O
	assert(SUCCEEDED(result));

	//�P�ʍs�����
	WorldTransform a;
	//���s���e�s��
	XMMATRIX ortho = XMMatrixOrthographicOffCenterLH(
		0.0f, WIN_WIDTH,
		WIN_HEIGHT, 0.0f,
		0.0f, 1.0f
	);

	//�������e�ϊ��s��̌v�Z
	XMMATRIX perspective = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),			//�㉺��p45�x
		(float)WIN_WIDTH / WIN_HEIGHT,//�A�X�y�N�g��
		0.1f, 1000.0f						//�O�[,���[
	);
	//matrix4�ɕϊ�
	matProjecsion = ConvertToMatrix4(perspective);

	//�r���[�ϊ��s��
	XMMATRIX xmmatView;
	XMFLOAT3 eye(0, 0, -100);	//���_���W
	XMFLOAT3 target(0, 0, 0);	//�����_���W
	XMFLOAT3 up(0, 1, 0);		//������x�N�g��
	xmmatView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	//matrix4�ɕϊ�
	matView = ConvertToMatrix4(xmmatView);

	//���[���h�ϊ��s��
	worldTransform.scale = { 1.0f,1.0f,1.0f };
	worldTransform.rotation = { 0.0f,1.0f,1.0f };
	worldTransform.trans = { 0.0f,0.0f,0.0f };
	worldTransform.SetWorldMat();

	constMapTransform->mat = worldTransform.matWorld * matView * matProjecsion;
}

void Object3d::Update()
{
	worldTransform.SetWorldMat();
	constMapTransform->mat = worldTransform.matWorld * matView * matProjecsion;
}

void Object3d::Scale(float x, float y, float z)
{
	worldTransform.scale = { Vector3(x,y,z) };
}

void Object3d::Rotate(float x, float y, float z)
{
	worldTransform.rotation = { Vector3(x,y,z) };
}

void Object3d::Trans(float x, float y, float z)
{
	worldTransform.trans = { Vector3(x,y,z) };
}

void Object3d::Draw(uint16_t _countofIndices)
{
	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	dxCom->commandList->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

	dxCom->commandList->DrawIndexedInstanced(_countofIndices, 1, 0, 0, 0); // �S�Ă̒��_���g���ĕ`��
}
