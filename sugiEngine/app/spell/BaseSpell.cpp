#include "BaseSpell.h"
#include "Player.h"
#include "ModelManager.h"
#include "ColliderManager.h"

void BaseSpell::Initialize(Vector3 pos, Vector3 vec)
{
	obj_.Initialize("sphere");

	vec_ = vec.normalize();

	//プレイヤーの少し前に出す
	obj_.pos = pos + vec_ * SPELL_LENGE;

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
		Dead();
	}

	BaseCol::Update(obj_.pos,obj_.scale);
	WorldTransUpdate();
}

void BaseSpell::Draw()
{
	if (spellType_ == SHOT) {
		obj_.Draw();
	}
}

void BaseSpell::WorldTransUpdate()
{
	obj_.Update();
}

void BaseSpell::Fire()
{
	isDead_ = false;
	BaseCol::Initialize(obj_.pos, obj_.scale, SPELL);
}

void BaseSpell::Explode()
{
	Dead();
}

bool BaseSpell::GetIsCalcCol()
{
	//当たり判定を行わない
	if (spellType_ == BUFF) {
		return false;
	}
	return true;
}

void BaseSpell::Dead()
{
	isDead_ = true;
	ColliderManager::GetInstance()->DeleteCollider(this);
}
