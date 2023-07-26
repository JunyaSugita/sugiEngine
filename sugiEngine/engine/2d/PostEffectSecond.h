#pragma once
#include <Windows.h>
#include "DXCommon.h"
#include "GrovalSetting.h"
#include "SugiMath.h"
#include "WorldTransform.h"
#include <string>

class PostEffectSecond
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	struct VertexSp {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
	};

	struct ConstBufferDataEffect {
		uint32_t blur;
		int32_t sigma;
		uint32_t invertColor;
		uint32_t border;
		uint32_t gray;
		uint32_t bloom;
		uint32_t closs4;
		uint32_t closs6;
		uint32_t closs8;
		uint32_t moveCloss;
	};

public:
	void Initialize(ID3D12Device* device);
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);
	void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

	void SetUp();

	static void SetClear() {
		sIsBlur = false;
		sIsInvertColor = false;
		sIsBorder = false;
		sIsGray = false;
		sIsBloom = false;
		sIsCloss4 = false;
		sIsCloss6 = false;
		sIsCloss8 = false;
		sIsMoveCloss = false;

		sIsDirty = true;
	}

	static void SetBlur(int32_t sigma = 5) {
		SetClear();
		sIsBlur = true;
		sSigma = sigma;
	}
	static void SetInvertColor() {
		SetClear();
		sIsInvertColor = true;
	}
	static void SetBorder() {
		SetClear();
		sIsBorder = true;
	}
	static void SetGray() {
		SetClear();
		sIsGray = true;
	}
	static void SetBloom() {
		SetClear();
		sIsBloom = true;
	}
	static void SetCloss4() {
		SetClear();
		sIsCloss4 = true;
	}
	static void SetCloss6() {
		SetClear();
		sIsCloss6 = true;
	}
	static void SetCloss8() {
		SetClear();
		sIsCloss8 = true;
	}
	static void SetMoveCloss() {
		SetClear();
		sIsMoveCloss = true;
	}

public:
	static const float CLEAR_COLOR[4];
	static const size_t MAX_SRV_COUNT = 2056;

	static bool sIsBlur;
	static int32_t sSigma;
	static bool sIsInvertColor;
	static bool sIsBorder;
	static bool sIsGray;
	static bool sIsBloom;
	static bool sIsCloss4;
	static bool sIsCloss6;
	static bool sIsCloss8;
	static bool sIsMoveCloss;

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
	DirectX::XMFLOAT4 color_ = { 1,1,1,1 };
	Vector2 size_ = { WIN_WIDTH,WIN_HEIGHT };
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