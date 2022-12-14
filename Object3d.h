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
#include "model.h"

using namespace std;

//定数バッファ用データ構造体B0
struct ConstBufferDataB0 {
	Matrix4 mat;
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

	static UINT incrementSize;
	static ComPtr<ID3D12Resource> constBuffMaterial;
	static uint16_t CountIndex;

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

	void SetModel(Model* model) {
		this->model = model;
	}

private:
	ComPtr<ID3D12Resource> constBuffB0 = nullptr;
	ConstBufferDataB0* constMapTransform = nullptr;
	WorldTransform worldTransform;

	Model* model = nullptr;

};

