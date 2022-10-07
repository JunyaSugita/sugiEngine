#pragma once
#include <Windows.h>
#include "WorldTransform.h"
#include "DXCommon.h"
#include "GrovalSetting.h"

//�萔�o�b�t�@�p�f�[�^�\����(3D�ϊ��s��)
struct ConstBufferDataTransform {
	Matrix4 mat;
};

class Object3d
{
public:
	void Initialize(DXCommon* dxCom);

	void Update();
	void Scale(float x,float y,float z);
	void Rotate(float x, float y, float z);
	void Trans(float x, float y, float z);

	void Draw(uint16_t _countofIndices);

private:
	ID3D12Resource* constBuffTransform = nullptr;
	ConstBufferDataTransform* constMapTransform = nullptr;
	Matrix4 matProjecsion;
	Matrix4 matView;
	WorldTransform worldTransform;

	DXCommon* dxCom = nullptr;
};

