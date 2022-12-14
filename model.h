#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <wrl.h>

#include "GrovalSetting.h"
#include "DXCommon.h"

class Model
{
public:
	//頂点データ構造体
	struct Vertex {
		XMFLOAT3 pos;	//xyz
		XMFLOAT3 normal;//法線ベクトル
		XMFLOAT2 uv;	//uv座標
	};

	//マテリアル
	struct Material {
		std::string name;	//マテリアル名
		XMFLOAT3 ambient;	//アンビエント影響度
		XMFLOAT3 diffuse;	//ディフューズ影響度
		XMFLOAT3 specular;	//スペキュラー影響度
		float alpha;		//アルファ
		std::string textureFilename;	//テクスチャファイル名
		//コンストラクタ
		Material() {
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

	//定数バッファ用データ構造体B1
	struct ConstBufferDataB1 {
		XMFLOAT3 ambient;	//アンビエント係数
		float pad1;			//パディング
		XMFLOAT3 diffuse;	//ディフューズ係数
		float pad2;			//パディング
		XMFLOAT3 specular;	//スペキュラー係数
		float alpha;		//アルファ
	};


public:
	static Model* LoadFromObj(const std::string& modelname);
	static void SetDevice(ID3D12Device* device) {
		Model::device = device;
	}

private:
	static ID3D12Device* device;

private:
	void LoatFromObjInternal(const std::string& modelname);

public:
	/// <summary>
	/// マテリアル読み込み
	/// </summary>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	bool LoadTexture(const std::string& directoryPath, const std::string& filename);

	void InitializeDescriptorHeap();

	void CreateBuffers();

	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootparamIndexMaterial);

private:
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
	Material material;
	//テクスチャバッファの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;

	ID3D12DescriptorHeap* srvHeap;
	//インデックスバッファの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff;
	// 頂点バッファの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
	D3D12_VERTEX_BUFFER_VIEW vbView;
	D3D12_INDEX_BUFFER_VIEW ibView;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1;
	ConstBufferDataB1* constMap1 = nullptr;
};

