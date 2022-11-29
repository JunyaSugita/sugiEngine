#pragma once
#include <Windows.h>
#include "DXCommon.h"
#include "GrovalSetting.h"
#include "GrovalSetting.h"
#include "WorldTransform.h"

#include <DirectXMath.h>
using namespace DirectX;



class Sprite
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
	static ComPtr<ID3D12Device> device;
	static ComPtr<ID3D12PipelineState> pipelineState;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootSignature;
	// 頂点バッファビューの作成
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	static XMFLOAT3 vertices[];
	static ComPtr<ID3D12GraphicsCommandList> cmdList;
public:
	void Initialize();
	void Draw();

private:
	D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
	D3D12_RESOURCE_DESC resDesc{};
	ComPtr<ID3D12Resource> vertBuff = nullptr;
};

