#pragma once
#include "GrovalSetting.h"
#include "WorldTransform.h"
#include "BaseObj.h"
#include "BaseCol.h"

struct DebuffM {
	bool isFire;
	int32_t fireTime;
	bool isThunder;
	int32_t thunderTime;
	bool isIce;
	int32_t iceTime;
};

class BaseEnemy {
public:
	virtual void Initialize(Vector3 pos);
	virtual void Update();
	virtual void Draw();
	virtual void WorldTransUpdate();

#pragma region inline群
	//inline群
	Vector3 GetPos() {
		return obj_.pos;
	}
	void SetCol(Vector3 vec) {
		col_.col.pos = vec;
	}
	void SetColX(float x) {
		col_.col.pos.x = x;
	}
	void SetColY(float y) {
		col_.col.pos.y = y;
	}
	void SetColZ(float z) {
		col_.col.pos.z = z;
	}
	void AddCol(Vector3 vec) {
		col_.col.pos += vec;
	}
	void AddColX(float x) {
		col_.col.pos.x += x;
	}
	void AddColZ(float z) {
		col_.col.pos.z += z;
	}
	bool GetIsDead() {
		return isDead_;
	}
	void SetIsDead() {
		isDead_ = true;
	}
	void ResetIsHit() {
		isHit_ = false;
	}
	BoxCol GetBoxCol() {
		return col_.col;
	}
	BoxCol GetOldBoxCol() {
		return col_.oldCol;
	}
	void SetIsStop() {
		isStop_ = true;
	}
	uint32_t GetLife() {
		return life_;
	}

	//デバッグ用
	
	// 敵の動きを止めたり動かしたり制御する
	static void SetIsDebugStop() {
		sIsDebugStop_ = (sIsDebugStop_ + 1) % 2;
	}

#pragma endregion

	/// <summary>
	/// デバフを付与
	/// </summary>
	/// <param name="debuff">デバフの種類番号</param>
	/// <param name="time">デバフを適応する時間/f</param>
	void SetDebuff(uint8_t debuff, uint32_t time);

	/// <summary>
	/// HPを減らし、エフェクトを出す
	/// </summary>
	/// <param name="subLife">hp減少量</param>
	/// <param name="effectNum">エフェクトを出す量</param>
	void SetIsHit(int32_t subLife = 1, int32_t effectNum = 0);

	/// <summary>
	/// デバフ状態になっているか
	/// </summary>
	/// <returns>デバフ状態になっているか</returns>
	bool isDebuff();

	/// <summary>
	/// 動ける状態か
	/// </summary>
	/// <returns>動ける状態か</returns>
	bool isCanMove();

	// シェイクで移動していた分を戻す
	void ResetShake();

	// 攻撃してプレイヤーにダメージを与える
	void SetIsAttack();

protected:
	virtual void Move() = 0;
	virtual void Attack() = 0;

	// プレイヤーの方向を向く
	void SetAngleToPlayer();

	/// <summary>
	/// 移動速度倍率
	/// </summary>
	/// <returns>倍率</returns>
	float GetSlow();

	/// <summary>
	/// hpを減らし、エフェクトを出す
	/// </summary>
	/// <param name="subLife">減らすhp量</param>
	/// <param name="effectNum">出すエフェクト量</param>
	void SubLife(int32_t subLife, int32_t effectNum);

	// デバフの効果を更新
	void UpdateDebuff();

	//自身をシェイクさせる
	void SetShake();
	
	//自身のコリジョンを設定
	void SetCol();

protected:
	const Vector2 UP = { 0,-1 };
	const float RADIAN = 180;

	//本体
	BaseObj obj_;

	//当たり判定
	BaseCol col_;
	float height_;

	//体力
	int32_t life_;

	//死んだかどうか
	bool isDead_;

	//プレイヤーとの距離を記録
	Vector2 toPlayer;

	//多段ヒットの回避フラグ
	bool isHit_;

	//デバフ
	DebuffM debuff_;

	//足を止めるか
	bool isStop_;

	//攻撃
	float attackTimer_;
	bool isAttack_;

	//回転速度
	float angleSpeed_;

	//デバッグ
	static bool sIsDebugStop_;
};