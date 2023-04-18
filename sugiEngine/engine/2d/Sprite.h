#pragma once
#include <Windows.h>
#include "DXCommon.h"
#include "GrovalSetting.h"
#include "GrovalSetting.h"
#include "WorldTransform.h"
#include "Vector2.h"

#include <DirectXMath.h>
using namespace DirectX;

struct VertexSp {
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};

class Sprite
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	
public:
	static void StaticInitialize(ID3D12Device* device);

	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	static void PostDraw();

	static uint32_t LoadTexture(const std::string& textureName);

private:
	//デバイス
	static ComPtr<ID3D12Device> device_;
	static ComPtr<ID3D12PipelineState> pipelineState;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootSignature;
	// 頂点バッファビューの作成

	static ComPtr<ID3D12GraphicsCommandList> cmdList_;

	//SRVの最大数
	static const size_t kMaxSRVCount = 2056;
	static std::array<ComPtr<ID3D12Resource>, kMaxSRVCount> textureBuffers_;

	static ComPtr<ID3D12DescriptorHeap> srvHeap;
	static UINT incrementSize;
	static uint32_t textureIndex;

private:
	void AdjustTextureSize();

public:
	void Initialize(uint32_t texNum);
	void Draw();

	void SetPos(float x, float y);

	void SetRotate(float r);

	void SetColor(float x, float y, float z, float w);

	void SetSize(float x, float y);

	void SetAnchorPoint(float x,float y);

	void SetFlipX(bool isFlip);
	void SetFlipY(bool isFlip);

	void SetIsView(bool is) {
		isView = is;
	}

	void SetUpVertex();

	void SetTexture(uint32_t texNum) {
		textureNum = texNum;
	};

	Vector2 GetTextureSize() {
		return textureSize_;
	}

	void SetTextureSize(float x, float y);

private:
	D3D12_HEAP_PROPERTIES heapProp_{}; // ヒープ設定
	D3D12_RESOURCE_DESC resDesc_{};
	ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	ID3D12Resource* constBuffTransform_ = nullptr;
	ConstBufferDataMaterial* constMapMaterial_ = nullptr;
	ConstBufferDataTransform* constMapTransform = nullptr;
	WorldTransform worldTransform;
	Vector2 pos;
	float rotate;
	XMFLOAT4 color = {1,1,1,1};
	Vector2 size = { 100.0f,100.0f };
	Vector2 anchorPoint = {0.0f,0.0f};
	bool isFlipX = false;
	bool isFlipY = false;
	bool isView = true;
	VertexSp vertices[4];
	D3D12_VERTEX_BUFFER_VIEW vbView;
	ComPtr<ID3D12Resource> constBuffMaterial;
	uint32_t textureNum;

	Vector2 textureLeftTop_ = { 0.0f,0.0f };
	Vector2 textureSize_ = { 100.0f,100.0f };
};

