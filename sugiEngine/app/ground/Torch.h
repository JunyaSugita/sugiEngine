/**
 * @file Torch.h
 * @brief フィールドに置くたいまつ
 */

#pragma once
#include "LightGroup.h"
#include "Model.h"
#include "BaseObj.h"

class Torch
{
public:
	void Initialize(const Vector3& pos, const Vector3& rot, const Vector3& scale, float atten);
	void Update();
	void Draw();

	//ライトグループのセット
	static void SetLight(LightGroup* lightGroup) {
		lightGroup_ = lightGroup;
	}

private:
	//角度調整
	const Vector3 PATCH_ROT = { 180 ,-90 ,90 };
	const Vector4 COLOR = { 0.2f,0.2f,0.2f,1 };
	const Vector3 COLOR_LIGHT = { 1,0.2f,0 };
	//アテン調整
	const float PATCH_ATTEN = 5;

	std::unique_ptr<Model> torchModel_;
	BaseObj torchObj_;

	//ライト関係
	static LightGroup* lightGroup_;
	int32_t useLightNum_;
	float stageAtten_;
};

