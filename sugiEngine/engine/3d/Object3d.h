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

//�萔�o�b�t�@�p�f�[�^�\����B0
struct ConstBufferDataB0 {
	//Matrix4 mat;
	XMMATRIX viewproj;
	XMMATRIX world;
	XMFLOAT3 cameraPos;
};

class Object3d
{
public:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	static void StaticInitialize(ID3D12Device* device);

	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	static void PostDraw();

	static Object3d* Create();

private:
	//�f�o�C�X
	static ID3D12Device* device_;

	static ID3D12GraphicsCommandList* cmdList_;
	static ComPtr<ID3D12PipelineState> pipelineState;
	static ComPtr<ID3D12RootSignature> rootSignature;

	static UINT incrementSize;
	static ComPtr<ID3D12Resource> constBuffMaterial;
	static uint16_t CountIndex;

public:
	bool Initialize();

	void Update();
	void Scale(Vector3 scale);
	void Rotate(Vector3 rot);
	void Trans(Vector3 pos);

	void Draw();

	void SetModel(Model* model) {
		model_ = model;
	}
	void SetWorldTransform(WorldTransform worldTransform) {
		worldTransform_ = worldTransform;
	}
	void SetColor(Vector4 color) {
		color_.x = color.x;
		color_.y = color.y;
		color_.z = color.z;
		color_.w = color.w;
	}

	Vector3 GetPos() {
		return worldTransform_.trans_;
	}

	static void SetLight(LightGroup* lightGroup) {
		lightGroup_ = lightGroup;
	}

private:
	ComPtr<ID3D12Resource> constBuffB0 = nullptr;
	ConstBufferDataB0* constMap = nullptr;
	WorldTransform worldTransform_;

	Model* model_ = nullptr;
	XMFLOAT4 color_ = {1,1,1,1};
	static LightGroup* lightGroup_;
};
