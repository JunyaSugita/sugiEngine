#pragma once
#include "Model.h"
#include <list>
#include "Vector3.h"
#include "Burst.h"
#include "Bolt.h"

class EffectManager final
{
private:
	EffectManager() = default;
	~EffectManager() = default;

public:
	//�R�s�[�R���X�g���N�^����
	EffectManager(const EffectManager& obj) = delete;
	//������Z�q�𖳌�
	EffectManager& operator=(const EffectManager& obj) = delete;

	static EffectManager* GetInstance();

public:
	void Initialize();
	void Update();
	void Draw();

	//��юU��G�t�F�N�g(���W,��)
	void BurstGenerate(const Vector3& pos, int32_t num,const Vector4& color, float range = 0.5f, float pow = 0.5f);
	void BoltGenerate(const Vector3& posS, const Vector3& posE, const Vector3& rot, const Vector4& color);
private:
	std::list<std::unique_ptr<Burst>> burst_;
	std::list<std::unique_ptr<Bolt>> bolt_;
};
