#pragma once
#include <Windows.h>
#include "WorldTransform.h"
#include "DXCommon.h"
#include "GrovalSetting.h"
#include <wrl.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

//���_�f�[�^�\����
struct Vertex {
	XMFLOAT3 pos;	//xyz
	XMFLOAT3 normal;//�@���x�N�g��
	XMFLOAT2 uv;	//uv���W
};

//�萔�o�b�t�@�p�f�[�^�\����B0
struct ConstBufferDataB0 {
	Matrix4 mat;
};

//�萔�o�b�t�@�p�f�[�^�\����B1
struct ConstBufferDataB1 {
	XMFLOAT3 ambient;	//�A���r�G���g�W��
	float pad1;			//�p�f�B���O
	XMFLOAT3 diffuse;	//�f�B�t���[�Y�W��
	float pad2;			//�p�f�B���O
	XMFLOAT3 specular;	//�X�y�L�����[�W��
	float alpha;		//�A���t�@
};


class Object3d
{
	//�}�e���A��
	struct Material {
		std::string name;	//�}�e���A����
		XMFLOAT3 ambient;	//�A���r�G���g�e���x
		XMFLOAT3 diffuse;	//�f�B�t���[�Y�e���x
		XMFLOAT3 specular;	//�X�y�L�����[�e���x
		float alpha;		//�A���t�@
		std::string textureFilename;	//�e�N�X�`���t�@�C����
		//�R���X�g���N�^
		Material() {
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

public:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	static void StaticInitialize(ID3D12Device* device);

	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	static void PostDraw();

	/// <summary>
	/// �}�e���A���ǂݍ���
	/// </summary>
	static void LoadMaterial(const string& directoryPath, const string& filename);

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	static bool LoadTexture(const string& directoryPath, const string& filename);

	static void SetCameraPos(Vector3 pos);
	static void SetCameraTarget(Vector3 pos);
	static void AddCameraPos(Vector3 pos);
	static void AddCameraTarget(Vector3 pos);

	static Object3d* Create();

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

	static vector<Vertex> vertices;
	static vector<unsigned short> indices;

	static Material material;

	static XMFLOAT3 eye;
	static XMFLOAT3 target;
	static XMFLOAT3 up;

public:
	bool Initialize();

	void Update();
	void Scale(float x, float y, float z);
	void Rotate(float x, float y, float z);
	void Trans(float x, float y, float z);

	void Draw();

private:
	ComPtr<ID3D12Resource> constBuffB0 = nullptr;
	ComPtr<ID3D12Resource> constBuffB1 = nullptr;

	ConstBufferDataB0* constMapTransform = nullptr;
	WorldTransform worldTransform;
	ConstBufferDataB1* constMap1 = nullptr;
};

