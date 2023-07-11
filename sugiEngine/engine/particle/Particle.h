#pragma once
#include <Windows.h>
#include "DXCommon.h"
#include "GrovalSetting.h"
#include "WorldTransform.h"
#include "Vector2.h"
#include <forward_list>

#include <DirectXMath.h>

struct PARTICLE {
	Vector3 position = {};
	float scale = 1.0f;
	float s_scale = 1.0f;
	float e_scale = 1.0f;
	Vector3 velocity = {};
	Vector3 accel = {};
	int32_t frame = 0;
	int32_t num_frame = 0;
};

class Particle
{
public:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	struct VertexSp {
		XMFLOAT3 pos;
		float scale;
	};

	struct ConstBuffB1 {
		XMMATRIX mat;
		XMMATRIX billboard;
	};

public:
	static void StaticInitialize(ID3D12Device* device);

	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	static void PostDraw();

	static uint32_t LoadTexture(const std::string& textureName);

protected:
	//�f�o�C�X
	static ComPtr<ID3D12Device> sDevice;
	static ComPtr<ID3D12PipelineState> sPipelineState;
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> sRootSignature;
	// ���_�o�b�t�@�r���[�̍쐬

	static ComPtr<ID3D12GraphicsCommandList> sCmdList;

	//SRV�̍ő吔
	static const size_t MAX_SRV_COUNT = 2056;
	static std::array<ComPtr<ID3D12Resource>, MAX_SRV_COUNT> sTextureBuffers;

	static ComPtr<ID3D12DescriptorHeap> sSrvHeap;
	static uint32_t sIncrementSize;
	static uint32_t sTextureIndex;

	static const uint32_t vertexCount = 1024;

private:
	void AdjustTextureSize();

public:
	void Initialize(uint32_t texNum);
	void Update();
	void Draw();

	void SetPos(float x, float y);

	void SetRotate(float r);

	void SetColor(float x, float y, float z, float w);

	void SetSize(float x, float y);

	void SetAnchorPoint(float x, float y);

	void SetFlipX(bool isFlip);
	void SetFlipY(bool isFlip);

	void SetIsView(bool is) {
		isView_ = is;
	}

	void SetUpVertex();

	void SetTexture(uint32_t texNum) {
		textureNum_ = texNum;
	};

	Vector2 GetTextureSize() {
		return textureSize_;
	}

	void SetTextureSize(float x, float y);

	void Add(int life, Vector3 pos, Vector3 velo, Vector3 accel,float start_scale,float end_scale);
protected:
	D3D12_HEAP_PROPERTIES heapProp_{}; // �q�[�v�ݒ�
	D3D12_RESOURCE_DESC resDesc_{};
	ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	ID3D12Resource* constBuffTransform_ = nullptr;
	ConstBufferDataMaterial* constMapMaterial_ = nullptr;
	ConstBuffB1* constMapTransform_ = nullptr;
	WorldTransform worldTransform_;
	Vector3 pos_;
	float rotate_;
	XMFLOAT4 color_ = { 1,1,1,1 };
	Vector2 size_ = { 100.0f,100.0f };
	Vector2 anchorPoint_ = { 0.5f,0.5f };
	bool isFlipX_ = false;
	bool isFlipY_ = false;
	bool isView_ = true;
	VertexSp vertices_[vertexCount];
	D3D12_VERTEX_BUFFER_VIEW vbView_;
	ComPtr<ID3D12Resource> constBuffMaterial_;
	uint32_t textureNum_;

	Vector2 textureLeftTop_ = { 0.0f,0.0f };
	Vector2 textureSize_ = { 100.0f,100.0f };

	std::forward_list<PARTICLE> particles_;
};

