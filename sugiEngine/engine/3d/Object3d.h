/**
 * @file Object3d.h
 * @brief obj
 */

#pragma once
#include <Windows.h>
#include "WorldTransform.h"
#include "DXCommon.h"
#include "GlobalSetting.h"
#include <wrl.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "model.h"
#include "Vector3.h"
#include "LightGroup.h"
#include "Camera.h"
#include "PostEffect.h"
#include "PostEffectSecond.h"

//定数バッファ用データ構造体B0
struct ConstBufferDataB0 {
	DirectX::XMMATRIX viewproj;
	DirectX::XMMATRIX world;
	DirectX::XMFLOAT3 cameraPos;
	float pad1;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT2 tiling;
	DirectX::XMFLOAT2 pad2;
	float simple;
	DirectX::XMFLOAT3 pad3;
	float effectCloss;
	DirectX::XMFLOAT3 pad4;
	float bloom;
};

class Object3d
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	static void StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList);

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
	void SetWorldTransform(WorldTransform& worldTransform) {
		worldTransform.SetWorldMat();
		worldTransform_ = worldTransform;
	}
	void SetColor(const Vector4& color) {
		color_.x = color.x;
		color_.y = color.y;
		color_.z = color.z;
		color_.w = color.w;
	}
	void SetColor(const Vector3& color,float alpha) {
		color_.x = color.x;
		color_.y = color.y;
		color_.z = color.z;
		color_.w = alpha;
	}
	void SetTiling(const Vector2& uv) {
		tiling_.x = uv.x;
		tiling_.y = uv.y;
	}

	Vector3 GetPos() {
		return worldTransform_.GetPos();
	}

	static void SetLight(LightGroup* lightGroup) {
		lightGroup_ = lightGroup;
	}

	void SetIsSimple() {
		isSimple_ = true;
	}
	void SetEffectCross(bool is) {
		effectCloss_ = is;
		PostEffect::SetMultiRenderTargetNum(2);
	}
	void SetIsBloom(bool is = true) {
		isBloom_ = is;
		PostEffect::SetMultiRenderTargetNum(3);
	}
private:
	//デバイス
	static ID3D12Device* sDevice;

	static ID3D12GraphicsCommandList* sCmdList;
	static ComPtr<ID3D12PipelineState> sPipelineState;
	static ComPtr<ID3D12RootSignature> sRootSignature;

	static uint32_t sIncrementSize;
	static ComPtr<ID3D12Resource> sConstBuffMaterial;
	static uint16_t sCountIndex;


private:
	ComPtr<ID3D12Resource> constBuffB0_ = nullptr;
	ConstBufferDataB0* constMap_ = nullptr;
	WorldTransform worldTransform_;

	Model* model_ = nullptr;
	DirectX::XMFLOAT4 color_;
	DirectX::XMFLOAT2 tiling_;
	static LightGroup* lightGroup_;

	bool isSimple_;
	bool effectCloss_;
	bool isBloom_;
};

