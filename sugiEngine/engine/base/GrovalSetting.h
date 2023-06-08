#pragma once
#include "Matrix4.h"

static const uint32_t WIN_WIDTH = 1280;
static const uint32_t WIN_HEIGHT = 720;

//定数バッファ用データ構造体(マテリアル)
struct ConstBufferDataMaterial {
	XMFLOAT4 color;	//色(RGBA)
};

//定数バッファ用データ構造体(3D変換行列)
struct ConstBufferDataTransform {
	Matrix4 mat;
};