#include "Wind.h"
#include "ParticleManager.h"
#include "ModelManager.h"
#include "Status.h"
#include "SpellManager.h"
#include <random>
#include <DirectXMath.h>

LightGroup* Wind::lightGroup_ = nullptr;

void Wind::Initialize(const Vector3& pos, const Vector3& vec)
{
	//平行移動するように
	BaseSpell::Initialize({ pos.x,0.3f,pos.z }, { vec.x,0,vec.z });

	obj_.obj->SetColor({ 1,0,1,1 });
	obj_.obj->SetEffectCross(true);
	obj_.obj->SetIsSimple();
	obj_.scale = { 0.5f,0.5f,0.5f };

	col_.pos = pos;
	col_.size = { 2.0f,0.1f,2.0f };

	spellData_ = Status::GetInstance()->GetSpellData(WIND);

	useLightNum_ = lightGroup_->SetPointLightGetNum();
	lightGroup_->SetPointLightColor(useLightNum_, { 0,3,0 });
	lightGroup_->SetPointLightAtten(useLightNum_, { 0.01f,0.01f,0.01f });
	lightGroup_->SetPointLightPos(useLightNum_, { pos.x, 1 ,pos.z });

	//ランダム
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());

	for (int i = 0; i < NUM_EFFECT; i++) {
		std::uniform_real_distribution<float> angle(0, PI * 2);
		angle_[i] = angle(engine);
		worldTransform_[i].parent_ = &obj_.worldTrans;
	}
	isPiercing_ = true;
}

void Wind::Update()
{
	//ランダム
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());

	BaseSpell::Update();

	obj_.rot.y += 5;
	obj_.Update();
	
	for (int i = 0; i < NUM_EFFECT; i++) {
		if (i % 5 == 0) {
			angle_[i] += 0.1f;
		}
		else if (i % 5 == 1) {
			angle_[i] += 0.2f;
		}
		else if (i % 5 == 2) {
			angle_[i] += 0.3f;
		}
		else if (i % 5 == 3) {
			angle_[i] += 0.4f;
		}
		worldTransform_[i].SetPos({ sin(angle_[i]) * 3,0,cos(angle_[i]) * 3 });
		worldTransform_[i].AddRotY(-angle_[i] * PI / 2);
		worldTransform_[i].SetWorldMat();
		if (!isHit_) {
			std::uniform_real_distribution<float> rand(0, 30);
			if (rand(engine) < 1.0f) {
				ParticleManager::GetInstance()->AddFromFile(P_WIND, { 0,0,0 }, &worldTransform_[i]);
			}
			else if (rand(engine) < 1.3f) {
				ParticleManager::GetInstance()->AddFromFile(P_WIND_SUB, { 0,0,0 }, &worldTransform_[i]);
			}
		}
	}

	if (isHit_) {
		lightGroup_->SetPointLightActive(useLightNum_, false);
	}

	lightGroup_->SetPointLightPos(useLightNum_, { obj_.pos.x, obj_.pos.y ,obj_.pos.z });
}

void Wind::Draw()
{
}

float Wind::GetKnockBackXZ()
{
	return 0.0f;
}

float Wind::GetKnockBackY()
{
	return 3.0f;
}