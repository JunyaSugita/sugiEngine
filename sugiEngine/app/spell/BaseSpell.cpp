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

	BaseCol::Update(obj_.pos,col_.size);
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
	BaseCol::Initialize(obj_.pos, col_.size, SPELL);
}

void BaseSpell::Explode()
{
	Dead();
}

void BaseSpell::OnCollision(BaseCol* a)
{
	if (a->GetColType() == WALL) {
		if (ColliderManager::GetInstance()->CheckHitBox(GetCol(), a->GetCol())) {
			SetIsHit();
		}
	}
	else if (a->GetColType() == ENEMY) {
		//まだ当たってない呪文と倒れている敵は判定しない
		if (!GetIsHit() && a->GetIsDown()) {
			return;
		}
		if (ColliderManager::GetInstance()->CheckHitBox(GetCol(), a->GetCol())) {
			SetIsHit();
		}
	}
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
