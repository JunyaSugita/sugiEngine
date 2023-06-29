#pragma once
#include "FireBall.h"

class SpellManager {
private:
	SpellManager();
	~SpellManager();

public:
	//�R�s�[�R���X�g���N�^����
	SpellManager(const SpellManager& obj) = delete;
	//������Z�q�𖳌�
	SpellManager& operator=(const SpellManager& obj) = delete;

	static SpellManager* GetInstance();

public:
	void Initialize();
	void Update();
	void Draw();

	void ChargeFireBall();

public:
	const float TIME_CHARGE_FIREBALL = 1.0f * 60;

private:
	int32_t chargeTime = 0;

	std::list<std::unique_ptr<FireBall>> fireBalls_;
};