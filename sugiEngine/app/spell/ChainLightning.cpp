#include "ChainLightning.h"
#include "ColliderManager.h"
#include "ParticleManager.h"
#include "ModelManager.h"

void ChainLightning::Initialize(Vector3 pos, Vector3 vec)
{
	obj_.Initialize("sphere");
	obj_.obj->SetColor({ 1,0,0,1 });

	col_.Initialize();


	obj_.pos = pos;

	vec_ = vec.normalize();

	col_.col.pos = pos;
	col_.col.size = { 1,1,1 };

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

	if (!isDead_) {
		for (int i = 0; i < 50; i++) {
			obj_.pos += vec_ * SPEED_MOVE;
			SetCol();
			WorldTransUpdate();

			ParticleManager::GetInstance()->AddFromFile(P_LIGHTNING, obj_.pos);

			if (ColliderManager::GetInstance()->CheckHitEnemyToChainLightning()) {
				break;
			}
			if (obj_.pos.y <= 0) {
				break;
			}
		}
	}
}

void ChainLightning::Draw()
{
	obj_.Draw();
	if (ColliderManager::GetInstance()->GetIsShowHitBox()) {
		col_.Draw();
	}
}

void ChainLightning::Fire()
{
	isDead_ = false;
}

void ChainLightning::SetCol()
{
	col_.SetCol(obj_.pos);
	col_.col.size = { obj_.scale.x,obj_.scale.y, obj_.scale.x };
}

void ChainLightning::WorldTransUpdate()
{
	obj_.Update();
	col_.Update();
}
