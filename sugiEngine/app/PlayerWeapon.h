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
	//コピーコンストラクタ無効
	PlayerWeapon(const PlayerWeapon& obj) = delete;
	//代入演算子を無効
	PlayerWeapon& operator=(const PlayerWeapon& obj) = delete;

	static PlayerWeapon* GetInstance();

public:
	void Initialize();
	void Update(bool isAttack);
	void Draw();

private:
	void WorldTransUpdate();
	void SetWorldTrans();

private:
	//本体
	WorldTransform worldTrans_;
	Vector3 pos_;
	Vector3 rot_;
	Vector3 scale_;

	//本体のモデル関係
	std::unique_ptr <Model> model_;
	std::unique_ptr <Object3d> obj_;
};
