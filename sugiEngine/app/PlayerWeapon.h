#pragma once
#include "SugiMath.h"
#include "Model.h"
#include "Object3d.h"
#include "WorldTransform.h"

class PlayerWeapon final{
private:
	PlayerWeapon();
	~PlayerWeapon();

public:
	//�R�s�[�R���X�g���N�^����
	PlayerWeapon(const PlayerWeapon& obj) = delete;
	//������Z�q�𖳌�
	PlayerWeapon& operator=(const PlayerWeapon& obj) = delete;

	static PlayerWeapon* GetInstance();

public:
	void Initialize();
	void Update();
	void Draw();

private:
	void WorldTransUpdate();
	void SetWorldTrans();

private:
	//�{��
	WorldTransform worldTrans_;
	Vector3 pos_;
	Vector3 rot_;
	Vector3 scale_;

	std::unique_ptr <Model> model_;
	std::unique_ptr <Object3d> obj_;
};
