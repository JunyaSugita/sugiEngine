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

//定数バッファ用データ構造体B0
struct ConstBufferDataB0 {
	Matrix4 mat;
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


class Object3d
{
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

public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	static void StaticInitialize(ID3D12Device* device);

	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	static void PostDraw();

	/// <summary>
	/// マテリアル読み込み
	/// </summary>
	static void LoadMaterial(const string& directoryPath, const string& filename);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	static bool LoadTexture(const string& directoryPath, const string& filename);

	static void SetCameraPos(Vector3 pos);
	static void SetCameraTarget(Vector3 pos);
	static void AddCameraPos(Vector3 pos);
	static void AddCameraTarget(Vector3 pos);

	static Object3d* Create();

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

	static Material material;

	static XMFLOAT3 eye;
	static XMFLOAT3 target;
	static XMFLOAT3 up;

public:
	bool Initialize();

	void Update();
	void Scale(float x, float y, float z);
	void Rotate(float x, float y, float z);
	void Trans(float x, float y, float z);

	void Draw();

private:
	ComPtr<ID3D12Resource> constBuffB0 = nullptr;
	ComPtr<ID3D12Resource> constBuffB1 = nullptr;

	ConstBufferDataB0* constMapTransform = nullptr;
	WorldTransform worldTransform;
	ConstBufferDataB1* constMap1 = nullptr;
};

