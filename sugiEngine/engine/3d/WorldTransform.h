#pragma once
#include "Matrix4.h"
#include "Vector3.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include <string>
#include <DirectXMath.h>
#include <DirectXTex.h>
using namespace DirectX;

class WorldTransform {
public:
	//�A�t�B���ϊ����
	Vector3 scale = { 1,1,1 };
	Vector3 rot = { 0,0,0 };
	Vector3 pos = { 0,0,0 };

	//���[���h�ϊ��s��
	Matrix4 matWorld;

	//�e�̃I�u�W�F�N�g�ւ̃|�C���^
	WorldTransform* parent = nullptr;

private:
	void SetMatScale(Matrix4& matScale, const Vector3& scale);

	void SetMatTrans(Matrix4& matTrans, const Vector3& trans);

	void SetMatRot(Matrix4& matRot, const Vector3& rot, const char way);

public:
	void SetWorldMat();
};

