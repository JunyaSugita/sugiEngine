/**
 * @file BaseCol.h
 * @brief 当たり判定の汎用クラス
 */

#pragma once
#include "BaseObj.h"

enum ColType {
	PLAYER,
	ENEMY,
	WALL,
	SPELL,
	GOAL,

	COL_TYPE_END
};

struct Col {
	Vector3 pos;
	Vector3 size = {1,1,1};
};

struct DownState {
	float slow = 1.0f;
	float damage = 0;
};

class BaseCol{
public:
	void Initialize(Vector3 pos,Vector3 scale,int32_t colType,float gap = 0);
	void Update(Vector3 pos,Vector3 scale);
	void Draw();

	void SetCol(Vector3 pos, Vector3 scale);
	void SetOldCol();

	//当たり判定の押し出しを反映
	virtual void HitChangePos();
	//当たったフラグを立てる
	virtual void SetIsHit();
	//当たっているか取得
	virtual bool GetIsHit();
	//hpを減らす
	virtual void SubLife(float subLife, bool isParticle = false);
	//ダメージを与える
	virtual float GetDamage();
	//デバフを貰う
	virtual void SetDebuff(int32_t debuff, int32_t time);
	//デバフを与える
	virtual int32_t GetDebuff();
	//動きを止める
	virtual void SetIsStop();
	//攻撃した判定にする
	virtual void SetIsAttack();
	//ダウンしているか取得
	virtual bool GetIsDown();
	//ダウンしている時にプレイヤーに与える影響
	virtual void DownHitPlayer();
	//ダウンしている時に敵に与える影響
	virtual DownState GetDownHitEnemy();
	virtual void SetDownHitEnemy(DownState state);
	//位置取得
	virtual Vector3 GetPos();
	//シェイクをセット
	virtual void SetShakeTime(int32_t time);
	//ゴールした
	virtual void SetClear();
	//add系のvirtual
	virtual void AddColX(float num);
	virtual void AddColZ(float num);
	//敵のシリアル取得
	virtual int32_t GetSerial();
	
	//ゲッター
	int32_t GetColType() {
		return colType_;
	}
	Col GetCol() {
		return col_;
	}
	Col GetOldCol() {
		return oldCol_;
	}

	//セッター
	void SetColX(float x) {
		col_.pos.x = x;
	}
	void SetColY(float y) {
		col_.pos.y = y;
	}
	void SetColZ(float z) {
		col_.pos.z = z;
	}

protected:
	BaseObj colObj_;
	int32_t colType_;

	Col col_;
	Col oldCol_;
	float gap_;
};
