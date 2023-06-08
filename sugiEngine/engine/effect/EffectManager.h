#pragma once
#include "Model.h"
#include "Object3d.h"
#include <list>
#include "Vector3.h"
#include "Burst.h"

class EffectManager
{
public:
	void Initialize();
	void Update();
	void Draw();

	//飛び散りエフェクト(座標,数)
	void BurstGenerate(Vector3 pos, uint32_t num,Vector4 color, float range = 0.5f, float pow = 0.5f);
private:
	std::list<std::unique_ptr<Burst>> burst_;
};
