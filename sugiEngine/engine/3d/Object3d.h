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
#include "Vector3.h"
#include "LightGroup.h"
#include "Camera.h"

using namespace std;

//定数バッファ用データ構造体B0
struct ConstBufferDataB0 {
	//Matrix4 mat;
	XMMATRIX viewproj;
	XMMATRIX world;
	XMFLOAT3 cameraPos;
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

	static std::unique_ptr<Object3d> Create();

public:
	bool Initialize();

	void Update();
	void Scale(const Vector3& scale);
	void Rotate(const Vector3& rot);
	void Trans(const Vector3& pos);

	void Draw();

	void SetModel(Model* model) {
		model_ = model;
	}
	void SetWorldTransform(const WorldTransform& worldTransform) {
		worldTransform_ = worldTransform;
	}
	void SetColor(const Vector4& color) {
		color_.x = color.x;
		color_.y = color.y;
		color_.z = color.z;
		color_.w = color.w;
	}

	Vector3 GetPos() {
		return worldTransform_.GetPos();
	}

	static void SetLight(LightGroup* lightGroup) {
		lightGroup_ = lightGroup;
	}
private:
	//デバイス
	static ID3D12Device* sDevice;

	static ID3D12GraphicsCommandList* sCmdList;
	static ComPtr<ID3D12PipelineState> sPipelineState;
	static ComPtr<ID3D12RootSignature> sRootSignature;

	static UINT sIncrementSize;
	static ComPtr<ID3D12Resource> sConstBuffMaterial;
	static uint16_t sCountIndex;


private:
	ComPtr<ID3D12Resource> constBuffB0_ = nullptr;
	ConstBufferDataB0* constMap_ = nullptr;
	WorldTransform worldTransform_;

	Model* model_ = nullptr;
	XMFLOAT4 color_ = {1,1,1,1};
	static LightGroup* lightGroup_;
};

