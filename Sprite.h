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

	static void LoadTexture();

private:
	//デバイス
	static ComPtr<ID3D12Device> device;
	static ComPtr<ID3D12PipelineState> pipelineState;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootSignature;
	// 頂点バッファビューの作成
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	static ComPtr<ID3D12GraphicsCommandList> cmdList;
	static ComPtr<ID3D12Resource> constBuffMaterial;

	//SRVの最大数
	static const size_t kMaxSRVCount = 2056;
	//std::array<ComPtr<ID3D12Resource>, kMaxSRVCount> textureBuffers_;

	//テクスチャバッファの生成
	static ComPtr<ID3D12Resource> texBuff;
	//テクスチャバッファ2の生成
	static ComPtr<ID3D12Resource> texBuff2;
	static ComPtr<ID3D12DescriptorHeap> srvHeap;
	static UINT incrementSize;

public:
	void Initialize();
	void Draw(UINT texNum);

	void Pos(float x, float y);

	void Rotate(float r);

	void Color(float x, float y, float z, float w);

	void Size(float x, float y);

	void SetAnchorPoint(float x,float y);

	void FlipX(bool isFlip);
	void FlipY(bool isFlip);

	void SetIsView(bool is) {
		isView = is;
	}

	void SetUpVertex();

private:
	D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
	D3D12_RESOURCE_DESC resDesc{};
	ComPtr<ID3D12Resource> vertBuff = nullptr;
	ID3D12Resource* constBuffTransform = nullptr;
	ConstBufferDataMaterial* constMapMaterial = nullptr;
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
};

