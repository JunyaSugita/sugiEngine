#pragma once
#include <Windows.h>
#include "DXCommon.h"
#include "GrovalSetting.h"
#include "SugiMath.h"
#include "WorldTransform.h"
#include <string>

class PostEffect
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	struct VertexSp {
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};

public:
	void Initialize(ID3D12Device* device);
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);
	void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

public:
	static const float clearColor[4];
	static const size_t kMaxSRVCount = 2056;
private:
	ComPtr<ID3D12Resource> texBuff;
	ComPtr<ID3D12DescriptorHeap>descHeapSRV;
	ComPtr<ID3D12Resource> depthBuff;
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
	ComPtr<ID3D12Resource> textureBuffer_;
	ComPtr<ID3D12PipelineState> pipelineState;
	ComPtr<ID3D12RootSignature> rootSignature;
	ComPtr<ID3D12DescriptorHeap> srvHeap;

	D3D12_HEAP_PROPERTIES heapProp_{}; // ヒープ設定
	D3D12_RESOURCE_DESC resDesc_{};
	ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	ID3D12Resource* constBuffTransform_ = nullptr;
	ConstBufferDataMaterial* constMapMaterial_ = nullptr;
	ConstBufferDataTransform* constMapTransform_ = nullptr;
	WorldTransform worldTransform_;
	Vector2 pos_;
	float rotate_;
	XMFLOAT4 color_ = { 1,1,1,1 };
	Vector2 size_ = { WIN_WIDTH / 2,WIN_HEIGHT / 2};
	Vector2 anchorPoint_ = { 0.0f,0.0f };
	bool isFlipX_ = false;
	bool isFlipY_ = false;
	bool isView_ = true;
	VertexSp vertices_[4];
	D3D12_VERTEX_BUFFER_VIEW vbView_;
	ComPtr<ID3D12Resource> constBuffMaterial_;
	uint32_t textureNum_;

	Vector2 textureLeftTop_ = { 0.0f,0.0f };
	Vector2 textureSize_ = { WIN_WIDTH,WIN_HEIGHT };
};

