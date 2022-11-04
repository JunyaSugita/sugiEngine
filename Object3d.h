#pragma once
#include <Windows.h>
#include "WorldTransform.h"
#include "DXCommon.h"
#include "GrovalSetting.h"
#include <wrl.h>

//�萔�o�b�t�@�p�f�[�^�\����(3D�ϊ��s��)
struct ConstBufferDataTransform {
	Matrix4 mat;
};

//���_�f�[�^�\����
struct Vertex {
	XMFLOAT3 pos;	//xyz
	XMFLOAT3 normal;//�@���x�N�g��
	XMFLOAT2 uv;	//uv���W
};

class Object3d
{
private:
	static void StaticInitialize(ID3D12Device* device);

private:
	//�f�o�C�X
	static ID3D12Device* device;
	//���s���e�s��
	static XMMATRIX ortho;
	//�������e�ϊ��s��̌v�Z
	static XMMATRIX perspective;

	static WorldTransform worldTransform;
	static Matrix4 matProjecsion;
	static Matrix4 matView;

public:
	void Initialize(DXCommon* dxCom);

	void Update();
	void Scale(float x,float y,float z);
	void Rotate(float x, float y, float z);
	void Trans(float x, float y, float z);

	void Draw(uint16_t _countofIndices);

public:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
private:
	ComPtr<ID3D12Resource> constBuffTransform = nullptr;
	ConstBufferDataTransform* constMapTransform = nullptr;

	DXCommon* dxCom = nullptr;
};

