#pragma once
#include "Matrix4.h"

static const uint16_t WIN_WIDTH = 1280;
static const uint16_t WIN_HEIGHT = 720;

static const uint8_t MULTI_RENDAR_TARGET_NUM = 5;

//�萔�o�b�t�@�p�f�[�^�\����(�}�e���A��)
struct ConstBufferDataMaterial {
	XMFLOAT4 color;	//�F(RGBA)
};

//�萔�o�b�t�@�p�f�[�^�\����(3D�ϊ��s��)
struct ConstBufferDataTransform {
	Matrix4 mat;
};