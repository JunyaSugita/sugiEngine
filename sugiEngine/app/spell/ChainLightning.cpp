#include "ChainLightning.h"
#include "Particle.h"
#include "ModelManager.h"
#include "EffectManager.h"
#include "Player.h"
#include "ColliderManager.h"
#include "Status.h"

void ChainLightning::Initialize(const Vector3& pos, const Vector3& vec)
{
	obj_.Initialize("sphere");
	obj_.obj->SetColor({ 1,0,0,1 });

	obj_.pos = pos;

	Vector3 tempVec = vec;
	vec_ = tempVec.normalize();

	WorldTransUpdate();

	isDead_ = true;
	spellData_ = Status::GetInstance()->GetSpellData(CHAIN_LIGHTNING);
	isHit_ = false;

	alpha_ = 1.0f;
}

void ChainLightning::Update()
{
	if (--time_ <= 0) {
		isDead_ = true;
	}

	Vector3 posS = obj_.pos;

	if (!isDead_) {
		if (ColliderManager::GetInstance()->CheckHitEnemyToRay(Camera::GetInstance()->GetEye(), vec_)) {
			Vector3 HitPos = ColliderManager::GetInstance()->CheckHitPosEnemyToRay(Camera::GetInstance()->GetEye(), vec_);
			EffectManager::GetInstance()->BoltGenerate(posS, HitPos, { (Player::GetInstance()->GetCameraAngle().y) * -1,Player::GetInstance()->GetCameraAngle().x,0 }, COLOR);

			ColliderManager::GetInstance()->LightningEnemyToEnemy(HitPos);
		}
		else {
			for (int i = 0; i < MAX_LENGE; i++) {
				obj_.pos += vec_ * spellData_.speed;
				WorldTransUpdate();

				if (obj_.pos.y <= 0) {
					break;
				}
			}
			EffectManager::GetInstance()->BoltGenerate(posS, obj_.pos, { (Player::GetInstance()->GetCameraAngle().y) * -1,Player::GetInstance()->GetCameraAngle().x,0 }, COLOR);
		}
	}
}

void ChainLightning::Draw()
{
	//obj_.Draw();
}

void ChainLightning::Fire()
{
	isDead_ = false;
}

void ChainLightning::WorldTransUpdate()
{
	obj_.Update();
}
