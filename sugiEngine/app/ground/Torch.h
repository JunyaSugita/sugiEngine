#pragma once
#include "LightGroup.h"
#include "Model.h"
#include "BaseObj.h"

class Torch
{
public:
	void Initialize(Vector3 pos, Vector3 rot, Vector3 scale, float atten);
	void Update();
	void Draw();

	static void SetLight(LightGroup* lightGroup) {
		lightGroup_ = lightGroup;
	}

private:
	std::unique_ptr<Model> torchModel_;
	BaseObj torchObj_;

	//ライト関係
	static LightGroup* lightGroup_;
	int32_t useLightNum_;
	float stageAtten_;
};

