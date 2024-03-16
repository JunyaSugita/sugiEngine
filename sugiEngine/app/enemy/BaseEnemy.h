/**
 * @file BaseEnemy.h
 * @brief 敵のベースクラス
 */
#pragma once
#include "GlobalSetting.h"
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

class BaseEnemy : public BaseCol {

public:
	virtual void Initialize(const std::string& name, const Vector3& pos);
	virtual void Update();

	//動けない時の処理
	virtual void DontMoveUpdate();
	//一度目の描画
	virtual void Draw();
	//半透明などの描画
	virtual void DrawTransparent();
	//オブジェクトの移動
	virtual void WorldTransUpdate();
	//当たった時の処理
	void OnCollision(BaseCol* a)override;

	//死んだあとプレイヤーに当たった時の反応
	void DownHitPlayer()override;

	//死んだあと他の敵に当たった時の反応
	DownState GetDownHitEnemy() override;
	void SetDownHitEnemy(const DownState& state) override;

#pragma region inline群
	//inline群
	Vector3 GetPos() override{
		return obj_.pos;
	}
	void SetCol(Vector3 vec) {
		col_.pos = vec;
	}
	void SetColX(float x) {
		col_.pos.x = x;
	}
	void SetColY(float y) {
		col_.pos.y = y;
	}
	void SetColZ(float z) {
		col_.pos.z = z;
	}
	void AddCol(Vector3 vec) {
		col_.pos += vec;
	}
	void AddColX(float x) override{
		col_.pos.x += x;
	}
	void AddColZ(float z) override{
		col_.pos.z += z;
	}
	bool GetIsDead() {
		return isDead_;
	}
	bool GetIsDown() override {
		return isDown_;
	}
	void SetIsDead() {
		isDead_ = true;
	}
	void ResetIsHit() {
		isHit_ = false;
	}
	Col GetBoxCol() {
		return col_;
	}
	Col GetOldBoxCol() {
		return oldCol_;
	}
	void SetIsStop() override {
		isStop_ = true;
	}
	float GetLife() {
		return life_;
	}
	int32_t GetSerial() override{
		return serial_;
	}

	/// <summary>
	/// 弱点時に体の色を変える
	/// </summary>
	virtual void WeakBodyColor();

	//デバッグ用

	// 敵の動きを止めたり動かしたり制御する
	static void ToggleIsAllStop() {
		sIsDebugStop_ = (sIsDebugStop_ + 1) % 2;
	}
	//全ての敵の動きを止める
	static void SetIsAllStop(bool is) {
		sIsDebugStop_ = is;
	}
	//ライトセット
	static void SetLight(LightGroup* light) {
		light_ = light;
	}

#pragma endregion

	/// <summary>
	/// デバフを付与
	/// </summary>
	/// <param name="debuff">デバフの種類番号</param>
	/// <param name="time">デバフを適応する時間/f</param>
	void SetDebuff(int32_t debuff, int32_t time) override;

	/// <summary>
	/// HPを減らし、エフェクトを出す
	/// </summary>
	/// <param name="subLife">hp減少量</param>]
	/// <param name="subLife">パーティクルを出すか</param>
	void SetIsHit(float subLife, bool isParticle = false);

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
	virtual void ResetShake();

	// 攻撃してプレイヤーにダメージを与える
	void SetIsAttack()override;

	// 移動速度をセット
	void SetSlow(float slow) {
		slow_ = slow;
	}

	//長時間のシェイク
	void SetShakeTime(int32_t time) override{
		shakeTime_ = time;
	}

	//ノックバックのセット
	void SetKnockBack(Vector3 vec) override;

protected:
	virtual void Move() = 0;
	virtual void Attack() = 0;
	//パーティクルの出し方
	virtual void PopDebuffFireParticle();
	//死んだ時の反応
	virtual void Down();
	//ノックバック
	virtual void KnockBack();

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
	/// <param name="subLife">パーティクルを出すかどうか</param>
	void SubLife(float subLife, bool isParticle = false) override;

	// デバフの効果を更新
	void UpdateDebuff();

	//自身のコリジョンを設定
	void SetCol();

	//自身をシェイクさせる
	void SetShake();

	//当たり判定適応
	void HitChangePos()override;

	/// <summary>
	/// 死ぬ時の処理
	/// </summary>
	void Dead();

	//重力
	void Gravity();

protected:
	const Vector2 UP = { 0,-1 };
	const float RADIAN = 180;
	const int32_t TIME_DOWN = 60 * 5;
	const Vector4 COLOR_ICE = { 0.5f,1,0.5f,0.5f };
	const Vector3 START_ROT = { 0,90,0 };
	const Vector3 COLOR_FIRE = { 0.5f,0.2f,0 };
	const Vector3 LIGHT_ATTEN = { 0.005f,0.005f,0.005f };
	//何フレームごとにバーンダメージをくらうか
	const int32_t TIME_FIRE_BURN = 40;
	//一回のバーンダメージでくらうダメージ量
	const float DAMAGE_FIRE_BURN = 1;
	const float MAX_SHAKE = 0.5f;
	//シェイク量調整
	const float CALC_SHAKE = 100;
	//火のチカチカの最大値と最小値
	const float MAX_ATTEN_FIRE = 0.006f;
	const float MIN_ATTEN_FIRE = 0.004f;
	//敵同士の押し出し処理
	const float PUSH_LEN = 0.01f;
	//呪文に当たった時のデバフの時間
	const int32_t DEBUFF_TIME = 3;
	const int32_t TIME_SHAKE = 10;
	//ノックバックの硬直時間
	const int32_t TIME_KNOCKBACK = 30;
	//重力
	const float GRAVITY = 0.7f;
	//呪文によるY軸ノックバック
	float spellKnockY_ = 0;

	//本体
	BaseObj obj_;
	//サイズ
	Vector2 size_;

	//iceモデル
	BaseObj iceObj_;

	//体力
	float life_;

	//死んだかどうか
	bool isDead_;
	bool isDown_;
	int32_t downTimer_;

	//プレイヤーとの距離を記録
	Vector2 toPlayer;

	//多段ヒットの回避フラグ
	bool isHit_;

	//ノックバック
	bool isKnockBack_;
	Vector3 knockBackVec_;
	int32_t knockBackTimer_;

	//デバフ
	DebuffM debuff_;

	//ライト
	static LightGroup* light_;
	int32_t lightNum_ = -1;

	//足を止めるか
	bool isStop_;

	//攻撃
	float attackTimer_;
	bool isAttack_;

	//回転速度
	float angleSpeed_;

	//デバッグ
	static bool sIsDebugStop_;

	//移動速度減少
	float slow_;

	//自身をシェイクさせている時間
	int32_t shakeTime_;

	//敵が起動しているか
	bool isStart_;

	//シリアル
	int32_t serial_ = 0;

	//最初のフレームか
	bool isFirstFrame_ = false;
};
