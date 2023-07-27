#include "EffectManager.h"

EffectManager* EffectManager::GetInstance()
{
	static EffectManager instance;

	return &instance;
}

void EffectManager::Initialize()
{
	Burst::OneTimeInitialize();
}

void EffectManager::Update()
{
	//��юU��G�t�F�N�g
	burst_.remove_if([](std::unique_ptr<Burst>& burst) {return burst->IsDead(); });
	for (std::unique_ptr<Burst>& burst : burst_) {
		burst->Update();
	}
}

void EffectManager::Draw()
{
	for (std::unique_ptr<Burst>& burst : burst_) {
		burst->Draw();
	}
}

//��юU��G�t�F�N�g
void EffectManager::BurstGenerate(const Vector3& pos, int32_t num,const Vector4& color, float range, float pow)
{
	for (int32_t i = 0; i < num; i++) {
		std::unique_ptr<Burst> newBurst = std::make_unique<Burst>();
		newBurst->Initialize(pos, color, range, pow);
		burst_.push_back(std::move(newBurst));
	}
}
