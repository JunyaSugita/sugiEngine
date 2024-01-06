/**
 * @file Model.h
 * @brief objのモデル
 */

#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <wrl.h>
#include <unordered_map>

#include "GlobalSetting.h"
#include "DXCommon.h"
#include "Vector4.h"

class Model
{
public:
	//頂点データ構造体
	struct Vertex {
		DirectX::XMFLOAT3 pos;	//xyz
		DirectX::XMFLOAT3 normal;//法線ベクトル
		DirectX::XMFLOAT2 uv;	//uv座標
	};

	//マテリアル
	struct Material {
		std::string name;	//マテリアル名
		DirectX::XMFLOAT3 ambient;	//アンビエント影響度
		DirectX::XMFLOAT3 diffuse;	//ディフューズ影響度
		DirectX::XMFLOAT3 specular;	//スペキュラー影響度
		float alpha;		//アルファ
		DirectX::XMFLOAT4 color;
		std::string textureFilename;	//テクスチャファイル名
		//コンストラクタ
		Material() {
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
			color = { 1,1,1,1 };
		}
	};

	//定数バッファ用データ構造体B1
	struct ConstBufferDataB1 {
		DirectX::XMFLOAT3 ambient;	//アンビエント係数
		float pad1;			//パディング
		DirectX::XMFLOAT3 diffuse;	//ディフューズ係数
		float pad2;			//パディング
		DirectX::XMFLOAT3 specular;	//スペキュラー係数
		float alpha;		//アルファ
	};


public:
	static Model* LoadFromObj(const std::string& modelname,bool smoothing = false);
	static void SetDevice(ID3D12Device* device) {
		sDevice = device;
	}
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1_;
private:
	void LoatFromObjInternal(const std::string& modelname, bool smoothing = false);

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

	void Draw(ID3D12GraphicsCommandList* cmdList, uint32_t rootparamIndexMaterial);

	inline size_t GetVertexCount() {
		return vertices_.size();
	}

	void AddSmoothData(unsigned short indexPosition, unsigned short indexVertex);
	void CalculateSmoothedVertexNormals();

private:
	static ID3D12Device* sDevice;

private:
	std::vector<Vertex> vertices_;
	std::vector<unsigned short> indices_;
	Material material_;
	//テクスチャバッファの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff_;

	ID3D12DescriptorHeap* srvHeap_;
	//インデックスバッファの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff_;
	// 頂点バッファの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;
	D3D12_VERTEX_BUFFER_VIEW vbView_;
	D3D12_INDEX_BUFFER_VIEW ibView_;

	ConstBufferDataB1* constMap1_ = nullptr;
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData_;
};

