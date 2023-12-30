#include "BaseSpell.h"
#include "Player.h"
#include "ModelManager.h"

void BaseSpell::Initialize(Vector3 pos, Vector3 vec)
{
	obj_.Initialize("sphere");

	cols_.Initialize();

	vec_ = vec.normalize();

	//プレイヤーの少し前に出す
	obj_.pos = pos + vec_ * SPELL_LENGE;

	cols_.col.pos = pos;

	isDead_ = true;
	isHit_ = false;
}

void BaseSpell::Update()
{
	//攻撃の種類によって動きを変える
	if (spellType_ == SHOT) {
		if (!isHit_) {
			obj_.pos += vec_ * moveSpeed_;
			if (obj_.pos.y <= 0) {
				isHit_ = true;
			}
		}
		else {
			Explode();
		}
	}

	if (--time_ <= 0) {
		isDead_ = true;
	}

	SetCol();

	WorldTransUpdate();
}

void BaseSpell::Draw()
{
	if (spellType_ == SHOT) {
		obj_.Draw();
	}

	if (ColliderManager::GetInstance()->GetIsShowHitBox()) {
		cols_.Draw();
	}
}

void BaseSpell::SetCol()
{
	cols_.col.size = { obj_.scale.x,obj_.scale.y, obj_.scale.x };
	cols_.SetCol(obj_.pos);
}

void BaseSpell::WorldTransUpdate()
{
	obj_.Update();
	cols_.Update();
}

void BaseSpell::Fire()
{
	isDead_ = false;
}

void BaseSpell::Explode()
{
	isDead_ = true;
}

bool BaseSpell::GetIsCalcCol()
{
	//当たり判定を行わない
	if (spellType_ == BUFF) {
		return false;
	}
	return true;
}
