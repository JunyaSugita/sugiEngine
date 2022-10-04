#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#include <string>
#include <DirectXTex.h>

#include "Groval.h"
#include "matrix4.h"

class Sprite
{
public:
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

public:
	static void StaticInitialize(ID3D12Device* device);
	static void LoadTexture();

public:
	// デバイス
	static ID3D12Device* device;
	static int descriptorSize;

	//テクスチャの最大数
	static const int srvConst = 512;
};

