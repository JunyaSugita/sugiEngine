#pragma once
#include "GrovalSetting.h"
#include "BaseObj.h"
#include "BaseCol.h"


enum SpellType {
	SHOT,
	BUFF
};

enum DebuffType {
	D_NONE,
	D_FIRE,
	D_SLOW,
	D_STAN,
};

class BaseSpell {
public:
	virtual void Initialize(Vector3 pos, Vector3 vec = Vector3());
	virtual void Update();
	virtual void Draw();

	virtual void SetCol();
	virtual void WorldTransUpdate();
	virtual void Fire();
	virtual void Explode();

public:
	bool GetIsDead() {
		return isDead_;
	}
	void SetIsDead() {
		isDead_ = true;
	};
	void SetIsHit() {
		isHit_ = true;
	}
	BoxCol GetBoxCol() {
		return col_.col;
	}
	int32_t GetDamage() {
		return damage_;
	}
	int32_t GetDebuff() {
		return debuffType_;
	}

protected:
	//�ʒu�����
	BaseObj obj_;
	//�����蔻��
	BaseCol col_;
	//�\���t���O
	bool isDead_;
	//������܂ł̎��ԊǗ�
	int32_t time_;
	//���������t���O
	bool isHit_;
	//�i�s����
	Vector3 vec_ = {0,0,0};
	//�ړ��X�s�[�h
	float moveSpeed_ = 0;
	//�����̃^�C�v
	int32_t spellType_;

	//�_���[�W(��)
	int32_t damage_ = 0;
	//�f�o�t
	int32_t debuffType_ = D_NONE;
};