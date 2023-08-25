#pragma once
#include "SugiMath.h"
#include "WorldTransform.h"
#include "BaseObj.h"

class PlayerWeapon final {
private:
	PlayerWeapon() = default;
	~PlayerWeapon() = default;

public:
	//�R�s�[�R���X�g���N�^����
	PlayerWeapon(const PlayerWeapon& obj) = delete;
	//������Z�q�𖳌�
	PlayerWeapon& operator=(const PlayerWeapon& obj) = delete;

	static PlayerWeapon* GetInstance();

public:
	void Initialize();
	void Update(bool isAttack, bool isAttackOn);
	void Draw();

	void NormalMove();
	void SpellMove();
	void ChargeMove();
	void ItemMove();
	void AttackMove(bool isAttackOn);
	void AttackCol();	//z�͔��a

	Vector3 GetPos() {
		return obj_.pos;
	}


	Vector3 GetHitPos() {
		return hitPos_;
	}

	bool GetIsAt() {
		return isAt_;
	}

private:
	void WorldTransUpdate();

public:
	const float SPEED_MOVE = 0.5f;
	const float ATTACK_RADIUS = 3.0f;
	const float ATTACK_LENGTH = 5;

private:
	//�{��
	BaseObj obj_;

	//��̃I�[�u
	BaseObj orbObj_;

	//�U������
	bool isAt_;

	//�����蔻��
	WorldTransform hitWorldTrans_;
	Vector3 hitPos_;
	Vector3 hitRot_;
	Vector3 hitScale_;

	//�����蔻��̃��f���֌W
	std::unique_ptr <Model> hitModel_;
	std::unique_ptr <Object3d> hitObj_;

	float easeTimer_ = 0;

	//�q�[�����̈ʒu�ړ�
	float healY;
	float healRot;

	//����̎��o��
	float y = 0;
	
	float alpha_;
};
