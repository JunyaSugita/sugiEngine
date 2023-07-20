#pragma once
#include "SugiMath.h"
#include "WorldTransform.h"
#include "SpellManager.h"

class Player final{
private:
	Player();
	~Player();
public:
	//コピーコンストラクタ無効
	Player(const Player& obj) = delete;
	//代入演算子を無効
	Player& operator=(const Player& obj) = delete;

	static Player* GetInstance();

public:
	void Initialize();
	void Update();
	void Draw();

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
	uint32_t GetLife() {
		return life_;
	}
	void SubLife() {
		life_--;
	}

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
	uint32_t GetPresetSpell(){
		return presetSpell_;
	}

	BoxCol GetBoxCol() {
		return boxCol_;
	}

	BoxCol GetOldBoxCol() {
		return oldBoxCol_;
	}

	void WorldTransUpdate();

private:
	void Move();
	void CameraMove();
	void Attack();
	

public:
	const Vector3 CAMERA_EYE = { 0.0f,5.0f,0.0f };//プレイヤーの目線調整
	const float SPEED_MOVE = 0.3f;	//プレイヤーのスピード
	const float SPEED_SLOW = 0.5f;	//プレイヤーがslow状態でのスピードの倍率
	const float SPEED_CAMERA = 3.0f;	//カメラのスピード
	const float TIME_ATTACK_START_NORMAL = 0.3f * 60.0f;//通常攻撃開始から攻撃判定が出るまでの時間
	const float TIME_ATTACK_END_NORMAL = 0.75f * 60.0f;//通常攻撃開始から攻撃判定が無くなるまでの時間
	const float TIME_ATTACK_NORMAL = 0.8f * 60.0f;	//通常攻撃開始から終了までのスピード

private:
	WorldTransform worldTrans_;
	Vector3 pos_;
	Vector3 rot_;
	Vector3 scale_;
	Vector2 cameraAngle_;//カメラ角度
	Vector3 frontVec_;//正面ベクトル
	Vector3 rightVec_;//右ベクトル

	int32_t life_;//体力

	bool isAttack_;//攻撃フラグ
	bool isSpell_;//魔法フラグ
	float attackTime_;//攻撃時間

	//装備呪文
	uint32_t presetSpell_;
	float spellAngle_;

	//当たり判定
	BoxCol boxCol_;
	BoxCol oldBoxCol_;
};

float Radian(float r);
