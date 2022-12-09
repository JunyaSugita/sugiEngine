#pragma once
#include <Windows.h>
#include "WorldTransform.h"
#include "DXCommon.h"
#include "GrovalSetting.h"
#include <wrl.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

//頂点データ構造体
struct Vertex {
	XMFLOAT3 pos;	//xyz
	XMFLOAT3 normal;//法線ベクトル
	XMFLOAT2 uv;	//uv座標
};

class Object3d
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	static void StaticInitialize(ID3D12Device* device);

	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	static void PostDraw();

private:
	//デバイス
	static ID3D12Device* device;
	//並行投影行列
	static XMMATRIX ortho;
	//透視投影変換行列の計算
	static XMMATRIX perspective;

	static Matrix4 matProjecsion;
	static Matrix4 matView;

	static ID3D12GraphicsCommandList* cmdList;
	static ComPtr<ID3D12PipelineState> pipelineState;
	static ComPtr<ID3D12RootSignature> rootSignature;
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	static D3D12_INDEX_BUFFER_VIEW ibView;

	static ID3D12DescriptorHeap* srvHeap;
	static UINT incrementSize;
	static ComPtr<ID3D12Resource> constBuffMaterial;
	static uint16_t CountIndex;

	//インデックスバッファの生成
	static ComPtr<ID3D12Resource> indexBuff;
	//テクスチャバッファの生成
	static ComPtr<ID3D12Resource> texBuff;
	//テクスチャバッファ2の生成
	static ComPtr<ID3D12Resource> texBuff2;
	// 頂点バッファの生成
	static ComPtr<ID3D12Resource> vertBuff;

	static vector<Vertex> vertices;
	static vector<unsigned short> indices;

public:
	void Initialize();

	void Update();
	void Scale(float x,float y,float z);
	void Rotate(float x, float y, float z);
	void Trans(float x, float y, float z);

	void Draw(UINT texNum);

private:
	ComPtr<ID3D12Resource> constBuffTransform = nullptr;
	ConstBufferDataTransform* constMapTransform = nullptr;
	WorldTransform worldTransform;
};

