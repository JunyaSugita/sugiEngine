#include "ChainLightning.h"
#include "ColliderManager.h"
#include "ParticleManager.h"
#include "ModelManager.h"
#include "EffectManager.h"
#include "Player.h"

void ChainLightning::Initialize(Vector3 pos, Vector3 vec)
{
	obj_.Initialize("sphere");
	obj_.obj->SetColor({ 1,0,0,1 });

	cols_.Initialize();


	obj_.pos = pos;

	vec_ = vec.normalize();

	cols_.col.pos = pos;
	cols_.col.size = { 1,1,1 };

	WorldTransUpdate();

	isDead_ = true;
	time_ = TIME_ALIVE;
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
		for (int i = 0; i < MAX_LENGE; i++) {
			obj_.pos += vec_ * SPEED_MOVE;
			SetCol();
			WorldTransUpdate();

			if (ColliderManager::GetInstance()->CheckHitEnemyToChainLightning()) {
				break;
			}
			if (obj_.pos.y <= 0) {
				break;
			}
		}
		EffectManager::GetInstance()->BoltGenerate(posS, obj_.pos, { (Player::GetInstance()->GetCameraAngle().y) * -1,Player::GetInstance()->GetCameraAngle().x,0 }, COLOR);
	}
}

void ChainLightning::Draw()
{
	obj_.Draw();
	if (ColliderManager::GetInstance()->GetIsShowHitBox()) {
		cols_.Draw();
	}
}

void ChainLightning::Fire()
{
	isDead_ = false;
}

void ChainLightning::SetCol()
{
	cols_.SetCol(obj_.pos);
	cols_.col.size = { obj_.scale.x,obj_.scale.y, obj_.scale.x };
}

void ChainLightning::WorldTransUpdate()
{
	obj_.Update();
	cols_.Update();
}
