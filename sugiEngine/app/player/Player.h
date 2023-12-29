/**
 * @file Player.h
 * @brief プレイヤーの操作
 */

#pragma once
#include "SugiMath.h"
#include "WorldTransform.h"
#include "SpellManager.h"
#include "Sprite.h"

class Player final{
private:
	Player() = default;
	~Player() = default;
public:
	//コピーコンストラクタ無効
	Player(const Player& obj) = delete;
	//代入演算子を無効
	Player& operator=(const Player& obj) = delete;

	static Player* GetInstance();

public:
	void Initialize();
	void GameInitialize();
	void Update();
	void Draw();
	void SpDraw();
	void ChargeSpell(int32_t num);

	//pos
	Vector3 GetPos() {
		return pos_;
	}
	void SetPos(const Vector3& pos) {
		pos_ = pos;
	}
	void SetPosX(float pos) {
		pos_.x = pos;
	}
	void SetPosY(float pos) {
		pos_.y = pos;
	}
	void SetPosZ(float pos) {
		pos_.z = pos;
	}

	//life
	int32_t GetLife() {
		return life_;
	}
	int32_t GetMaxLife() {
		return MAX_LIFE;
	}
	void SubLife(int32_t num = 1000);

	//worldTrans
	WorldTransform& GetWorldTrans() {
		return worldTrans_;
	}

	//cameraAngle
	Vector2 GetCameraAngle() {
		return cameraAngle_;
	}

	//time 
	float GetTime() {
		return attackTime_;
	};

	bool GetIsCanAction();
	bool GetIsSpell() {
		return isSpell_;
	}

	void SetIsSpell(bool is) {
		isSpell_ = is;
	}

	void SetPresetSpell(uint32_t spellName) {
		presetSpell_ = spellName;
	}
	int32_t GetPresetSpell(){
		return presetSpell_;
	}

	BoxCol GetBoxCol() {
		return boxCol_;
	}

	BoxCol GetOldBoxCol() {
		return oldBoxCol_;
	}

	void WorldTransUpdate();

	//回復
	void AddHealNum(int32_t num) {
		healingLife_ += num;
	}
	int32_t GetHeal() {
		return healingLife_;
	}

	//無敵
	void SetInvincible() {
		isInvincible_ = (isInvincible_ + 1) % 2;
	}
	bool GetInvincible() {
		return isInvincible_;
	}

	//移動速度減少の乗算
	void SetSlow(float num) {
		slow_ = num;
	}

private:
	void Move();
	void CameraMove();
	void Attack();
	void HealLife();

public:
	const Vector3 CAMERA_EYE = { 0.0f,5.0f,0.0f };//プレイヤーの目線調整
	const float SPEED_MOVE = 0.5f;	//プレイヤーのスピード
	const float SPEED_SLOW = 0.5f;	//プレイヤーがslow状態でのスピードの倍率
	const float SPEED_CAMERA = 3.0f;	//カメラのスピード
	const float TIME_ATTACK_START_NORMAL = 0.1f * 60.0f;//通常攻撃開始から攻撃判定が出るまでの時間
	const float TIME_ATTACK_END_NORMAL = 0.45f * 60.0f;//通常攻撃開始から攻撃判定が無くなるまでの時間
	const float TIME_ATTACK_NORMAL = 0.5f * 60.0f;	//通常攻撃開始から終了までのスピード
	const float TIME_NAVE = 1.0f * 60;
	const int32_t MAX_LIFE = 10000;
	const float MAX_Y = 3.0f;
	//ダメージ演出が消えていくスピード
	const float SPEED_DAMAGE_UI = 0.01f;
	//画面シェイクの大きさ
	const float SHAKE_SIZE = 0.05f;
	//スティックの修正
	const float PATCH_STICK = 32768.0f;

private:
	WorldTransform worldTrans_;
	Vector3 pos_;
	Vector3 rot_;
	Vector3 scale_;
	Vector2 cameraAngle_;//カメラ角度
	Vector3 frontVec_;//正面ベクトル
	Vector3 rightVec_;//右ベクトル

	int32_t life_;//体力
	int32_t healingLife_;	//ポーションの回復
	int32_t exLife_;	//呪文の回復

	bool isAttack_;//攻撃フラグ
	bool isSpell_;//魔法フラグ
	float attackTime_;//攻撃時間

	//装備呪文
	int32_t presetSpell_;
	float spellAngle_;

	//当たり判定
	BoxCol boxCol_;
	BoxCol oldBoxCol_;

	//ダメージ演出
	int32_t damageTex_;
	Sprite damageSp_;
	float damageAlpha_;

	//無敵
	bool isInvincible_;

	//navePointのタイミング
	float naveTimer_;

	//移動速度低下処理
	float slow_;
};

float Radian(float r);
