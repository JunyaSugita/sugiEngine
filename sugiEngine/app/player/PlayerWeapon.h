/**
 * @file PlayerWeapon.h
 * @brief プレイヤーが手に持つ杖
 */

#pragma once
#include "SugiMath.h"
#include "WorldTransform.h"
#include "BaseObj.h"
#include "LightGroup.h"

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

	//なにもしていない時の動き
	void NormalMove();
	//呪文発射の動き
	void SpellMove();
	//呪文詠唱中の動き
	void ChargeMove();
	//殴り攻撃の動き
	void AttackMove(bool isAttackOn);
	//当たり判定移動
	void AttackCol();

	//座標取得
	Vector3 GetPos() {
		return obj_.pos;
	}
	Vector3 GetOrbPos() {
		return orbObj_.worldTrans.GetMatPos();
	}
	Vector3 GetHitPos() {
		return hitPos_;
	}

	//攻撃しているか
	bool GetIsAt() {
		return isAt_;
	}

	//ライトグループセット
	static void SetLight(LightGroup* lightGroup) {
		lightGroup_ = lightGroup;
	}

	//呪文詠唱時にパーティクルを出す
	void PopChargeParticle(int32_t num);

private:
	void WorldTransUpdate();

	//使用中の呪文によって出すパーティクルの色を変える
	void ChargeParticle(Vector4 color);

public:
	const float SPEED_MOVE = 0.5f;
	const float ATTACK_RADIUS = 3.0f;
	const float ATTACK_LENGTH = 5;

private:
	//武器の色
	const Vector3 COLOR_WEAPON = {1,1,1};
	const Vector3 COLOR_ORB = {0,1,1};
	const float ALPHA_ORB = 0.5f;
	//呪文の色
	const Vector4 COLOR_FIRE = { 0.2f,0.04f,0,1 };
	const Vector4 COLOR_THUNDER = { 0.2f,0,0.2f,1 };
	const Vector4 COLOR_ICE = { 0,0.04f,0.1f,1 };
	const Vector4 COLOR_DARK = { 0.2f,0,0.2f,1 };
	const Vector4 COLOR_DEF = { 0.2f,0.2f,0.2f,1 };
	//クリア時の透明になるスピード
	const float SPEED_ALPHA = 0.02f;
	//武器の高さ
	const float START_WEAPON_Y = 3;
	const float PATCH_WEAPON_Y = 3.5f;
	//攻撃後のアニメーション補間
	const float EASING_TIME = 20;
	const float PATCH_EASING_TIME = 19;
	const float PATCH_ATTACK_EASING_TIME = 30;
	//魔法詠唱のコヨーテ
	const float MAGIC_COYOTE = 0.9f;
	//武器に炎
	const Vector3 COLOR_FIRE_LIGHT = { 1,0.2f,0 };
	const Vector3 ATTEN_FIRE = { 0.001f,0.001f,0.001f };
	//武器の位置
	const float POS_WEAPON = 30;
	const float LEN_WEAPON = 4;
	const float LEN_MOVE_WEAPON = 2;
	//武器を戻す速度
	const float SPEED_REVERSE_WEAPON = 0.1f;
	//細かい調整
	const float PATCH_SPELLMOVE = 4.5f;
	const float PATCH_SPELLMOVE_LEN = 10;
	const float PATCH_CHARGEMOVE = 15;
	const float PATCH_ATTACKMOVE = 130;
	const float PATCH_ATTACKMOVE_EASING = 6;
	const float PATCH_ATTACKMOVE_LEN = 5;
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

	//武器の取り出し
	float weaponY = 0;
	
	float alpha_;

	static LightGroup* lightGroup_;
	int32_t useLightNum_ = -1;
};
