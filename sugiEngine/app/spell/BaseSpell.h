#pragma once
#include "GrovalSetting.h"
#include "BaseObj.h"
#include "BaseCol.h"


enum SpellType {
	SHOT,
	BUFF
};

class BaseSpell {
public:
	virtual void Initialize(Vector3 pos, Vector3 vec = Vector3());
	virtual void Update();
	virtual void Draw();

	virtual void SetCol();
	virtual void WorldTransUpdate();
	virtual void Fire();
	virtual void Explode();

public:
	bool GetIsDead() {
		return isDead_;
	}
	void SetIsDead() {
		isDead_ = true;
	};
	void SetIsHit() {
		isHit_ = true;
	}
	BoxCol GetBoxCol() {
		return col_.col;
	}

protected:
	//位置や実体
	BaseObj obj_;
	//当たり判定
	BaseCol col_;
	//表示フラグ
	bool isDead_;
	//消えるまでの時間管理
	int32_t time_;
	//当たったフラグ
	bool isHit_;
	//進行方向
	Vector3 vec_ = {0,0,0};
	//移動スピード
	float moveSpeed_ = 0;
	//呪文のタイプ
	int32_t spellType_;
};