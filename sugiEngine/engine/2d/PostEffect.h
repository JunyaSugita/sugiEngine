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

	struct ConstBufferDataEffect {
		uint32_t blur;
		uint32_t border;
		uint32_t gray;
		uint32_t bloom;
	};

public:
	void Initialize(ID3D12Device* device);
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);
	void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

	void SetUp();

	static void SetClear() {
		sIsBlur = false;
		sIsBorder = false;
		sIsGray = false;
		sIsBloom = false;

		sIsDirty = true;
	}

	static void SetBlur() {
		sIsBlur = true;
		sIsBorder = false;
		sIsGray = false;
		sIsBloom = false;

		sIsDirty = true;
	}
	static void SetBorder() {
		sIsBlur = false;
		sIsBorder = true;
		sIsGray = false;
		sIsBloom = false;

		sIsDirty = true;
	}
	static void SetGray() {
		sIsBlur = false;
		sIsBorder = false;
		sIsGray = true;
		sIsBloom = false;

		sIsDirty = true;
	}
	static void SetBloom() {
		sIsBlur = false;
		sIsBorder = false;
		sIsGray = false;
		sIsBloom = true;

		sIsDirty = true;
	}

public:
	static const float CLEAR_COLOR[4];
	static const size_t MAX_SRV_COUNT = 2056;

	static bool sIsBlur;
	static bool sIsBorder;
	static bool sIsGray;
	static bool sIsBloom;

	static bool sIsDirty;
private:
	ComPtr<ID3D12Resource> texBuff_[MULTI_RENDAR_TARGET_NUM];
	ComPtr<ID3D12DescriptorHeap>descHeapSRV_;
	ComPtr<ID3D12Resource> depthBuff_;
	ComPtr<ID3D12DescriptorHeap> descHeapRTV_;
	ComPtr<ID3D12DescriptorHeap> descHeapDSV_;
	ComPtr<ID3D12Resource> textureBuffer_;
	ComPtr<ID3D12PipelineState> pipelineState_;
	ComPtr<ID3D12RootSignature> rootSignature_;
	ComPtr<ID3D12DescriptorHeap> srvHeap_;
	ComPtr<ID3D12Device> device_;

	D3D12_HEAP_PROPERTIES heapProp_{}; // ヒープ設定
	D3D12_RESOURCE_DESC resDesc_{};
	ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	ConstBufferDataMaterial* constMapMaterial_ = nullptr;
	ConstBufferDataTransform* constMapTransform_ = nullptr;
	ConstBufferDataEffect* constMapEffect_ = nullptr;
	WorldTransform worldTransform_;
	Vector2 pos_ = { 0,0 };
	float rotate_ = 0; //弧度法
	XMFLOAT4 color_ = { 1,1,1,1 };
	Vector2 size_ = { WIN_WIDTH,WIN_HEIGHT};
	Vector2 anchorPoint_ = { 0,0 };	//0.0f ~ 1.0f
	bool isFlipX_ = false;
	bool isFlipY_ = false;
	bool isView_ = true;
	VertexSp vertices_[4];
	D3D12_VERTEX_BUFFER_VIEW vbView_;
	ComPtr<ID3D12Resource> constBuffTransform_ = nullptr;
	ComPtr<ID3D12Resource> constBuffMaterial_ = nullptr;
	ComPtr<ID3D12Resource> constBuffEffect_ = nullptr;
	uint32_t textureNum_;

	Vector2 textureLeftTop_ = { 0.0f,0.0f };
	Vector2 textureSize_ = { WIN_WIDTH,WIN_HEIGHT };
};

