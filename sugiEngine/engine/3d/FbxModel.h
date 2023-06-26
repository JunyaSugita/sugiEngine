#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>
#include <DirectXTex.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <fbxsdk.h>

struct Node
{
	std::string name;

	DirectX::XMVECTOR scaling = { 1,1,1,0 };
	DirectX::XMVECTOR rotation = { 0,0,0,0 };
	DirectX::XMVECTOR translation = { 0,0,0,1 };
	DirectX::XMMATRIX transform;
	DirectX::XMMATRIX globalTransform;

	//�e�m�[�h
	Node* parent = nullptr;
};

class FbxModel
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;
	using string = std::string;
	template <class T> using vector = std::vector<T>;

public:
	static const int MAX_BONE_INDICES = 4;

public:
	struct VertexPosNormalUvSkin {
		XMFLOAT3 pos;
		XMFLOAT3 normal;
		XMFLOAT2 uv;
		uint32_t boneIndex[MAX_BONE_INDICES];	//�ԍ�
		float boneWeight[MAX_BONE_INDICES];	//�d��
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

	struct Bone {
		string name;
		//�����p���̋t�s��
		XMMATRIX invInitializePose;
		//FBX���̃{�[�����
		FbxCluster* fbxCluster;

		//�R���X�g���N�^
		Bone(const string& name) {
			this->name = name;
		}
	};


public:
	friend class FbxLoader;

public:
	void CreateBuffers(ID3D12Device* device);
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void Finalize();

	const XMMATRIX& GetModelTransform() {
		return meshNode_->globalTransform;
	}

	std::vector<Bone>& GetBones() {
		return bones;
	}

	FbxScene* GetFbxScene() {
		return fbxScene;
	}

private:
	string name_;
	//�m�[�h�z��
	vector<Node> nodes_;

	Node* meshNode_ = nullptr;
	vector<VertexPosNormalUvSkin> vertices_;
	vector<unsigned short> indices_;

	//�}�e���A��
	XMFLOAT3 ambient_ = { 1,1,1 };
	XMFLOAT3 diffuse_ = { 1,1,1 };
	TexMetadata metadata_ = {};
	ScratchImage scratchImg_ = {};

	Material material_;

	ComPtr<ID3D12Resource> vertBuff_;
	ComPtr<ID3D12Resource> indexBuff_;
	ComPtr<ID3D12Resource> texBuff_;
	D3D12_VERTEX_BUFFER_VIEW vbView_ = {};
	D3D12_INDEX_BUFFER_VIEW ibView_ = {};
	ComPtr <ID3D12DescriptorHeap> descHeapSRV_;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1_;
	ConstBufferDataB1* constMap1_ = nullptr;

	//�{�[���z��
	std::vector<Bone> bones;
	//FBX�V�[��
	FbxScene* fbxScene = nullptr;
};

