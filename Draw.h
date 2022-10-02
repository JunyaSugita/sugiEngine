#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <string>
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include "Groval.h"
#include "WorldTransform.h"
#include "Matrix4.h"

//定数バッファ用データ構造体(マテリアル)
struct ConstBufferDataMaterial {
	XMFLOAT4 color;	//色(RGBA)
};

//定数バッファ用データ構造体(3D変換行列)
struct ConstBufferDataTransform {
	Matrix4 mat;
};
//頂点データ構造体
struct Vertex {
	XMFLOAT3 pos;	//xyz
	XMFLOAT2 uv;	//uv座標
};

class Draw{
public:
	void TriangleInisialize(ID3D12Device* device);
	void GraphInisialize(ID3D12Device* device);

	void TriangleUpdate(ID3D12GraphicsCommandList* commandList);
	void GraphUpdate(ID3D12GraphicsCommandList* commandList);
public:
	HRESULT result;
	// ルートシグネチャ
	ID3D12RootSignature* rootSignature;
	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	ID3D12PipelineState* pipelineState = nullptr;
	UINT TriangleCount;

	ID3D12Resource* constBuffMaterial = nullptr;
	ID3D12DescriptorHeap* srvHeap = nullptr;
	ID3D12Resource* constBuffTransform = nullptr;
	D3D12_INDEX_BUFFER_VIEW ibView{};
	UINT GraphCount;
};

