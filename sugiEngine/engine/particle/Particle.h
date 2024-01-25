/**
 * @file Particle.h
 * @brief パーティクルを管理するマネージャー
 */

#pragma once
#include <Windows.h>
#include "DXCommon.h"
#include "GlobalSetting.h"
#include "WorldTransform.h"
#include "SugiMath.h"
#include <forward_list>
#include "ParticleEditor.h"
#include <DirectXMath.h>

struct ParticleState {
	int32_t frame = 0;
	int32_t num_frame = 0;
	Vector3 originPos = {};
	Vector3 position = {};
	bool isRevers_ = false;
	Vector3 velocity = {};
	float speed = 1.0f;
	Vector3 accel = { 1,1,1 };
	float scale = 1.0f;
	Vector2 checkS = { 0,0 };
	Vector4 scales = { 1,1,1,1 };
	Vector3 gravity = {};
	Vector4 color = { 1,1,1,1 };
	Vector4 s_color = { 1,1,1,1 };
	float check1 = 0;
	Vector4 check1Color = { 1,1,1,1 };
	float check2 = 0;
	Vector4 check2Color = { 1,1,1,1 };
	Vector4 e_color = { 1,1,1,1 };
	int32_t postEffect = 0;
};

enum BLEND_TYPE {
	ALPHA,
	ADD,
	SUB,

	BLEND_TYPE_END
};

class Particle final
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	struct VertexSp {
		DirectX::XMFLOAT3 pos;
		float scale;
		DirectX::XMFLOAT4 color;
	};

	struct ConstBuffB1 {
		DirectX::XMMATRIX mat;
		DirectX::XMMATRIX billboard;
	};

public:
	static void StaticInitialize(ID3D12Device* device);

	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	static void PostDraw();

	uint32_t LoadTexture(std::string file);

private:
	//デバイス
	static ComPtr<ID3D12Device> sDevice;
	static ComPtr<ID3D12PipelineState> sPipelineState[BLEND_TYPE_END];
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> sRootSignature;
	// 頂点バッファビューの作成

	static ComPtr<ID3D12GraphicsCommandList> sCmdList;

	//SRVの最大数
	static const size_t MAX_SRV_COUNT = 2056;
	static std::array<ComPtr<ID3D12Resource>, MAX_SRV_COUNT> sTextureBuffers;

	static ComPtr<ID3D12DescriptorHeap> sSrvHeap;
	static uint32_t sIncrementSize;
	static uint32_t sTextureIndex;

	static const uint32_t vertexCount = 2048 * 8;

private:
	void AdjustTextureSize();

public:
	void Initialize(std::string textureName, int32_t blendType = ADD);
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

	Vector2 GetTextureSize() {
		return textureSize_;
	}
	int32_t GetBlendType() {
		return blendType_;
	}

	void SetTextureSize(float x, float y);

	void AddCircle(int life, Vector3 pos, bool isRevers, Vector3 velo, float speed, Vector3 accel, Vector3 gravity, Vector2 checkS, Vector4 scale, Vector4 sColor, float check1, Vector4 check1Color, float check2, Vector4 check2Color, Vector4 eColor, int32_t postEffect);
	void Add(Vector3 pos, EditFile data);
	void AddEditScaleAndColor(Vector3 pos, EditFile data, float scale, Vector4 color);

	void Clear();
protected:
	D3D12_HEAP_PROPERTIES heapProp_{}; // ヒープ設定
	D3D12_RESOURCE_DESC resDesc_{};
	ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	ID3D12Resource* constBuffTransform_ = nullptr;
	ConstBuffB1* constMapTransform_ = nullptr;
	WorldTransform worldTransform_;
	Vector3 pos_;
	float rotate_;
	DirectX::XMFLOAT4 color_ = { 1,1,1,1 };
	Vector2 size_ = { 100.0f,100.0f };
	Vector2 anchorPoint_ = { 0.5f,0.5f };
	bool isFlipX_ = false;
	bool isFlipY_ = false;
	bool isView_ = true;
	VertexSp vertices_[vertexCount];
	D3D12_VERTEX_BUFFER_VIEW vbView_;
	uint32_t textureNum_;

	Vector2 textureLeftTop_ = { 0.0f,0.0f };
	Vector2 textureSize_ = { 100.0f,100.0f };

	int32_t blendType_ = ALPHA;

	std::forward_list<ParticleState> particles_;
};


