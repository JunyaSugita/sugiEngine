#pragma once
#include "SugiMath.h"

struct BoxCol{
	Vector3 pos;
	float width;
	float height;
};

class ColliderManager {
private:
	ColliderManager();
	~ColliderManager();

public:
	//�R�s�[�R���X�g���N�^����
	ColliderManager(const ColliderManager& obj) = delete;
	//������Z�q�𖳌�
	ColliderManager& operator=(const ColliderManager& obj) = delete;

	static ColliderManager* GetInstance();


public:
	void Initialize();
	void Update();

	bool GetIsShowHitBox() {
		return isShowHitBox_;
	}
	void ChangeIsShowHitBox() {
		isShowHitBox_ = (isShowHitBox_ + 1) % 2;
	}

	bool ChakeHitEnemyToChainLightning();

private:
	bool ChackHitBox(BoxCol a, BoxCol b);


private:
	//�����蔻��\���̃t���O
	bool isShowHitBox_;
};