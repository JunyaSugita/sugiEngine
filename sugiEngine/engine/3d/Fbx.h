#pragma once
#pragma once
#include "FbxModel.h"
#include "Camera.h"
#include "WorldTransform.h"
#include "FbxLoader.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <string>

class Fbx
{
protected:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	static const int MAX_BONES = 32;

public:
	struct ConstBufferDataTransform {
		XMMATRIX viewproj;
		XMMATRIX world;
		XMFLOAT3 cameraPos;
	};

	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

public:
	static void SetDevice(ID3D12Device* device) {
		device_ = device;
	}
	static void SetCamera(Camera* camera) {
		camera_ = camera;
	}

	static void CreateGraphicsPipeline();

	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	static void PostDraw();

public:
	void Initialize();
	void Update();
	void Draw();

	void SetModel(FbxModel* model) {
		model_ = model;
	}

public:
	static ComPtr<ID3D12RootSignature> sRootSignature;

	static ComPtr<ID3D12PipelineState> sPipelineState;

	static ID3D12GraphicsCommandList* sCmdList;

private:
	static ID3D12Device* device_;
	static Camera* camera_;

	ComPtr<ID3D12Resource> constBuffTransform_;

	Vector3 scale_ = { 1,1,1 };
	Vector3 rotation_ = { 0,0,0 };
	Vector3 position_ = { 0,0,0 };
	WorldTransform worldTransform_;
	FbxModel* model_ = nullptr;

	//定数バッファ
	ComPtr<ID3D12Resource> constBuffSkin_;
};

