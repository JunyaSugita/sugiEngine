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

//�萔�o�b�t�@�p�f�[�^�\����(�}�e���A��)
struct ConstBufferDataMaterial {
	XMFLOAT4 color;	//�F(RGBA)
};

//���_�f�[�^�\����
struct Vertex {
	XMFLOAT3 pos;	//xyz
	XMFLOAT3 normal;//�@���x�N�g��
	XMFLOAT2 uv;	//uv���W
};

class Object3d
{
public:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	static void StaticInitialize(ID3D12Device* device);

	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	static void PostDraw();

private:
	//�f�o�C�X
	static ID3D12Device* device;
	//���s���e�s��
	static XMMATRIX ortho;
	//�������e�ϊ��s��̌v�Z
	static XMMATRIX perspective;

	static Matrix4 matProjecsion;
	static Matrix4 matView;

	static ID3D12GraphicsCommandList* cmdList;
	static ComPtr<ID3D12PipelineState> pipelineState;
	static ComPtr<ID3D12RootSignature> rootSignature;
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	static D3D12_INDEX_BUFFER_VIEW ibView;

	static ID3D12DescriptorHeap* srvHeap;
	static UINT incrementSize;
	static ComPtr<ID3D12Resource> constBuffMaterial;
	static uint16_t CountIndex;

	//�C���f�b�N�X�o�b�t�@�̐���
	static ComPtr<ID3D12Resource> indexBuff;
	//�e�N�X�`���o�b�t�@�̐���
	static ComPtr<ID3D12Resource> texBuff;
	//�e�N�X�`���o�b�t�@2�̐���
	static ComPtr<ID3D12Resource> texBuff2;
	// ���_�o�b�t�@�̐���
	static ComPtr<ID3D12Resource> vertBuff;

public:
	void Initialize();

	void Update();
	void Scale(float x,float y,float z);
	void Rotate(float x, float y, float z);
	void Trans(float x, float y, float z);

	void Draw();

private:
	ComPtr<ID3D12Resource> constBuffTransform = nullptr;
	ConstBufferDataTransform* constMapTransform = nullptr;
	WorldTransform worldTransform;
};

