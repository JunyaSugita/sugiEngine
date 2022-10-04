#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#include <string>
#include <DirectXTex.h>

#include "Groval.h"
#include "matrix4.h"

class Sprite
{
public:
	//�萔�o�b�t�@�p�f�[�^�\����(�}�e���A��)
	struct ConstBufferDataMaterial {
		XMFLOAT4 color;	//�F(RGBA)
	};

	//�萔�o�b�t�@�p�f�[�^�\����(3D�ϊ��s��)
	struct ConstBufferDataTransform {
		Matrix4 mat;
	};

	//���_�f�[�^�\����
	struct Vertex {
		XMFLOAT3 pos;	//xyz
		XMFLOAT2 uv;	//uv���W
	};

public:
	static void StaticInitialize(ID3D12Device* device);
	static void LoadTexture();

public:
	// �f�o�C�X
	static ID3D12Device* device;
	static int descriptorSize;

	//�e�N�X�`���̍ő吔
	static const int srvConst = 512;
};

