#pragma once
#include "SugiMath.h"

static const uint16_t WIN_WIDTH = 1280;
static const uint16_t WIN_HEIGHT = 720;

static const uint8_t MULTI_RENDAR_TARGET_NUM = 8;

//定数バッファ用データ構造体(マテリアル)
struct ConstBufferDataMaterial {
	DirectX::XMFLOAT4 color;	//色(RGBA)
};

//定数バッファ用データ構造体(3D変換行列)
struct ConstBufferDataTransform {
	Matrix4 mat;
};
