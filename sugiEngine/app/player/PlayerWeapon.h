#pragma once
#include "SugiMath.h"
#include "Model.h"
#include "Object3d.h"
#include "WorldTransform.h"

class PlayerWeapon final {
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
	void Update(bool isAttack, bool isAttackOn);
	void Draw();

	void NormalMove();
	void SpellMove();
	void ChargeMove();
	void AttackMove(bool isAttackOn);
	void AttackCol();	//zは半径

	Vector3 GetPos() {
		return pos_;
	}


	Vector3 GetHitPos() {
		return hitPos_;
	}

	bool GetIsAt() {
		return isAt_;
	}

private:
	void WorldTransUpdate();
	void SetWorldTrans();

public:
	const float SPEED_MOVE = 0.5f;
	const float ATTACK_RADIUS = 3;
	const float ATTACK_LENGTH = 4;

private:
	//本体
	WorldTransform worldTrans_;
	Vector3 pos_;
	Vector3 rot_;
	Vector3 scale_;

	//攻撃判定
	bool isAt_;

	//本体のモデル関係
	std::unique_ptr <Model> model_;
	std::unique_ptr <Object3d> obj_;

	//当たり判定
	WorldTransform hitWorldTrans_;
	Vector3 hitPos_;
	Vector3 hitRot_;
	Vector3 hitScale_;

	//当たり判定のモデル関係
	std::unique_ptr <Model> hitModel_;
	std::unique_ptr <Object3d> hitObj_;

	float timer_ = 0;
};
