#include "BaseSpell.h"
#include "Player.h"
#include "ModelManager.h"
#include "ColliderManager.h"

void BaseSpell::Initialize(const Vector3& pos, const Vector3& vec)
{
	obj_.Initialize("sphere");

	Vector3 tempVec = vec;
	vec_ = tempVec.normalize();

	//プレイヤーの少し前に出す
	obj_.pos = pos + vec_ * SPELL_LENGE;

	isDead_ = true;
	isHit_ = false;
	tempIsHit_ = false;
}

void BaseSpell::Update()
{
	if (tempIsHit_) {
		isHit_ = true;
	}

	//攻撃の種類によって動きを変える
	if (spellData_.spellType == SHOT) {
		if (!isHit_) {
			obj_.pos += vec_ * spellData_.speed;
			if (obj_.pos.y <= 0) {
				isHit_ = true;
			}
		}
		else {
			Explode();
		}
	}

	if (--spellData_.timeAlive <= 0) {
		Dead();
	}

	BaseCol::Update(obj_.pos, col_.size);
	WorldTransUpdate();
}

void BaseSpell::Draw()
{
	if (spellData_.spellType == SHOT) {
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
		//敵が貫通するか
		if (!isPiercing_) {
			//まだ当たってない呪文と倒れている敵は判定しない
			if (!GetIsHit() && a->GetIsDown()) {
				return;
			}
			if (ColliderManager::GetInstance()->CheckHitBox(GetCol(), a->GetCol())) {
				SetIsHit();
			}
		}
	}
}

bool BaseSpell::GetIsCalcCol()
{
	//当たり判定を行わない
	if (spellData_.spellType == BUFF) {
		return false;
	}
	return true;
}

void BaseSpell::Dead()
{
	isDead_ = true;
	ColliderManager::GetInstance()->DeleteCollider(this);
}
