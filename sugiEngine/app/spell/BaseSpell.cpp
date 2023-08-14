#include "BaseSpell.h"
#include "Player.h"
#include "ModelManager.h"

void BaseSpell::Initialize(Vector3 pos, Vector3 vec)
{
	obj_.Initialize("sphere");

	col_.Initialize();

	vec_ = vec.normalize();

	//プレイヤーの少し前に出す
	obj_.pos = pos + vec_ * 3;

	col_.col.pos = pos;

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
	else if (spellType_ == BUFF) {

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
		col_.Draw();
	}
}

void BaseSpell::SetCol()
{
	col_.col.size = { obj_.scale.x,obj_.scale.y, obj_.scale.x };
	col_.SetCol(obj_.pos);
}

void BaseSpell::WorldTransUpdate()
{
	obj_.Update();
	col_.Update();
}

void BaseSpell::Fire()
{
	isDead_ = false;
}

void BaseSpell::Explode()
{
	isDead_ = true;
}
