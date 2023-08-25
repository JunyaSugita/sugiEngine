#pragma once
#include "SugiMath.h"
#include "WorldTransform.h"
#include "BaseObj.h"

class PlayerWeapon final {
private:
	PlayerWeapon() = default;
	~PlayerWeapon() = default;

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
	void ItemMove();
	void AttackMove(bool isAttackOn);
	void AttackCol();	//zは半径

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
	//本体
	BaseObj obj_;

	//上のオーブ
	BaseObj orbObj_;

	//攻撃判定
	bool isAt_;

	//当たり判定
	WorldTransform hitWorldTrans_;
	Vector3 hitPos_;
	Vector3 hitRot_;
	Vector3 hitScale_;

	//当たり判定のモデル関係
	std::unique_ptr <Model> hitModel_;
	std::unique_ptr <Object3d> hitObj_;

	float easeTimer_ = 0;

	//ヒール時の位置移動
	float healY;
	float healRot;

	//武器の取り出し
	float y = 0;
	
	float alpha_;
};
