#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <wrl.h>
#include <unordered_map>

#include "GrovalSetting.h"
#include "DXCommon.h"
#include "Vector4.h"

class Model
{
public:
	//���_�f�[�^�\����
	struct Vertex {
		XMFLOAT3 pos;	//xyz
		XMFLOAT3 normal;//�@���x�N�g��
		XMFLOAT2 uv;	//uv���W
	};

	//�}�e���A��
	struct Material {
		std::string name;	//�}�e���A����
		XMFLOAT3 ambient;	//�A���r�G���g�e���x
		XMFLOAT3 diffuse;	//�f�B�t���[�Y�e���x
		XMFLOAT3 specular;	//�X�y�L�����[�e���x
		float alpha;		//�A���t�@
		XMFLOAT4 color;
		std::string textureFilename;	//�e�N�X�`���t�@�C����
		//�R���X�g���N�^
		Material() {
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
			color = { 1,1,1,1 };
		}
	};

	//�萔�o�b�t�@�p�f�[�^�\����B1
	struct ConstBufferDataB1 {
		XMFLOAT3 ambient;	//�A���r�G���g�W��
		float pad1;			//�p�f�B���O
		XMFLOAT3 diffuse;	//�f�B�t���[�Y�W��
		float pad2;			//�p�f�B���O
		XMFLOAT3 specular;	//�X�y�L�����[�W��
		float alpha;		//�A���t�@
		XMFLOAT4 color;
	};


public:
	static Model* LoadFromObj(const std::string& modelname,bool smoothing = false);
	static void SetDevice(ID3D12Device* device) {
		device_ = device;
	}

private:
	static ID3D12Device* device_;

private:
	void LoatFromObjInternal(const std::string& modelname, bool smoothing = false);

public:
	/// <summary>
	/// �}�e���A���ǂݍ���
	/// </summary>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	bool LoadTexture(const std::string& directoryPath, const std::string& filename);

	void InitializeDescriptorHeap();

	void CreateBuffers();

	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootparamIndexMaterial, XMFLOAT4 color);

	inline size_t GetVertexCount() {
		return vertices.size();
	}

	void AddSmoothData(unsigned short indexPosition, unsigned short indexVertex);
	void CalculateSmoothedVertexNormals();

private:
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
	Material material;
	//�e�N�X�`���o�b�t�@�̐���
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;

	ID3D12DescriptorHeap* srvHeap;
	//�C���f�b�N�X�o�b�t�@�̐���
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff;
	// ���_�o�b�t�@�̐���
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
	D3D12_VERTEX_BUFFER_VIEW vbView;
	D3D12_INDEX_BUFFER_VIEW ibView;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1;
	ConstBufferDataB1* constMap1 = nullptr;
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData;
};

