#pragma once
#include "Model.h"
#include "Object3d.h"
#include <list>
#include "Vector3.h"
#include "Burst.h"

class EffectManager final
{
private:
	EffectManager() = default;
	~EffectManager() = default;

public:
	//コピーコンストラクタ無効
	EffectManager(const EffectManager& obj) = delete;
	//代入演算子を無効
	EffectManager& operator=(const EffectManager& obj) = delete;

	static EffectManager* GetInstance();

public:
	void Initialize();
	void Update();
	void Draw();

	//飛び散りエフェクト(座標,数)
	void BurstGenerate(const Vector3& pos, int32_t num,const Vector4& color, float range = 0.5f, float pow = 0.5f);
private:
	std::list<std::unique_ptr<Burst>> burst_;
};
